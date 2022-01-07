#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <fstream> // used to save or load files physically to the harddrive
#include <string>
#include "Editor.h"
#include "Helpers.h"

using namespace std;

Editor::Editor() {

}
Editor::~Editor() {

}

bool g_failState = false;
bool g_LegendDisplay = true;

int levelCounter = 0;

void Editor::EditorMain()
{
    bool quitApp = false;
    do {
        int levelWidth = 5;
        int levelHeight = 5;
        int keyAmount = 0;
        string levelName = "";
        char* pLevel = nullptr; // initialize level layout as pointer array
        system("cls");
        cout << "Do you want to edit or create a new level?" << endl;
        cout << "  (1) Edit Level" << endl;
        cout << "  (2) New Level" << endl;
        cout << "  (3) Quit" << endl;
        cout << "\nYour choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            pLevel = LoadLevel(levelWidth, levelHeight, keyAmount, levelName);
            break;
        }
        case 2: {
            // ask user how large the level should be (x * y)
            GetLevelDimensions(levelWidth, levelHeight);

            // create pointer array based on the level dimensions
            delete[] pLevel;
            pLevel = new char[(long long)levelWidth * levelHeight];

            // fill array with empty spaces
            for (int i = 0; i < levelWidth * levelHeight; i++) {
                pLevel[i] = ' ';
            }
            break;
        }
        case 3: {
            quitApp = true;
            break;
        }
        }
        // display a warning if the level couldn't be opened/found in the LoadLevel function
        if (g_failState) {
            g_failState = false;
            if (levelCounter == 0) {
                hlp.SetColor(Color::Default);
                cout << "\n\nPlease make sure all 'LevelXX.txt' files are in the GameData folder. Press any key to return to the top menu...\n" << endl;
            }
            else {
                hlp.SetColor(Color::Warning);
                cout << "\n\nERROR: ";
                hlp.SetColor(Color::Default);
                cout << "File not found or unable to open file. Press any key to return to the top menu...\n" << endl;
            }
            char input = _getch();
        } // if level was loaded properly show a short dialogue and how many random keys the opened level had (as it won't be displayed later on)
        else if (!g_failState && !quitApp) {
            if (keyAmount > 0) {
                hlp.SetColor(Color::Green);
                cout << "\nINFO:";
                hlp.SetColor(Color::Default);
                if (keyAmount == 1) {
                    cout << " Level successfully loaded! There was [";
                    hlp.SetColor(Color::Yellow);
                    cout << keyAmount;
                    hlp.SetColor(Color::Default);
                    cout << "] randomized key assigned to it.\n\nPress any key to start editing..." << endl;
                }
                else {
                    cout << " Level successfully loaded! There were [";
                    hlp.SetColor(Color::Yellow);
                    cout << keyAmount;
                    hlp.SetColor(Color::Default);
                    cout << "] randomized keys assigned to it." << endl;
                    cout << "\n\nPress any key to start editing..." << endl;
                }

                if (_getch()) keyAmount = 0;
            }
            int cursorX = 0;
            int cursorY = 0;
            bool doneEditing = false;

            // main editing loop, runs until user presses escape
            system("cls");
            while (!doneEditing) {
                hlp.ClearScreen();
                DisplayLevel(pLevel, levelWidth, levelHeight, cursorX, cursorY, levelName);
                cout << endl;
                doneEditing = EditLevel(pLevel, cursorX, cursorY, levelWidth, levelHeight);
            }

            // display full level layout, then run the save function
            system("cls");
            DisplayLevel(pLevel, levelWidth, levelHeight, -1, -1, levelName);

            int saveCheck = 0;
            cout << "\n\nDo you want to save the level?" << endl;
            cout << "  (1) Yes" << endl;
            cout << "  (2) No" << endl;
            cout << "\nYour choice: ";
            cin >> saveCheck;
            if (saveCheck == 1) {
                SaveLevel(pLevel, levelWidth, levelHeight);
            }
            else {
                hlp.SetColor(Color::Yellow);
                cout << "\n\nINFO: ";
                hlp.SetColor(Color::Default);
                cout << "Level has not been saved and was discarded. Press any key to return to the top menu..." << endl;
                char input = _getch();
            }
        }
        g_LegendDisplay = true;
        delete[] pLevel;
    } while (!quitApp);
}

// load level from an existing text file
// list all existing files found in the folder, ask user only to input the level number
char* Editor::LoadLevel(int& width, int& height, int& keyAmount, string& levelName) {
    system("cls");
    cout << "\nExisting levels found: ";
    string levelNumber = "";
    ifstream levelFile;
    for (int i = 1; i <= 99; i++) { // loop through and check if any level between 01 and 99 already exists
        levelNumber = "GameData/Level.txt";
        if (i < 10) {
            levelNumber.insert(14, "0"); // add an additional 0 after "Level", only used for levels below 10
            levelNumber.insert(15, to_string(i));
        }
        else levelNumber.insert(14, to_string(i));
        levelFile.open(levelNumber);
        if (levelFile) {
            levelCounter++;
            cout << "[";
            hlp.SetColor(Color::Yellow);
            if (i < 10) {
                cout << "0";
            }
            cout << i;
            hlp.SetColor(Color::Default);
            cout << "]";
        }
        levelFile.close();
    }
    if (levelCounter > 0) {
        cout << "\n\nPlease enter the number of the level you want to edit (e.g. 01, 02, 15, 24, etc.): ";
        cin >> levelNumber;
        levelNumber.insert(0, "GameData/Level"); // add level at the beginning of the filename
        levelNumber.insert(levelNumber.size(), ".txt"); // add .txt at the end of the filename

        levelFile.open(levelNumber);
        if (!levelFile) {
            g_failState = true;
            return nullptr;
        }
        else {
            constexpr int tempSize = 99;
            char temp[tempSize];
            levelFile.getline(temp, tempSize, '\n'); // this will grab the number on the first line in the text file
            width = atoi(temp);
            levelFile.getline(temp, tempSize, '\n'); // this will grab the number on the second line in the text file
            height = atoi(temp);
            levelFile.getline(temp, tempSize, '\n'); // this will grab the number on the third line in the text file
            keyAmount = atoi(temp);
            levelFile.getline(temp, tempSize, '\n'); // this will grab the number on the fourth line in the text file
            levelName = temp;
            char* level = new char[(long long)width * height]; // create a new pointer array for the level layout characters
            levelFile.read(level, long long(width) * height); // this will read all characters from the third line in the text file (position 0 to width * height)
            // using a type cast above on width, otherwise it would throw a warning:
            // Warning C26451 Arithmetic overflow : Using operator '*' on a 4 byte value and then casting the result to a 8 byte value. Cast the value to the wider type before calling operator '*' to avoid overflow
            return level;
        }
    }
    else {
        hlp.ColoredText("NONE",Color::Warning);
        g_failState = true;
        return nullptr;
    }
}

// save level size and layout into a new text file (also check for existing files first)
void Editor::SaveLevel(char* pLevel, int width, int height) {
    int keyAmount = 0;
    cout << "\nHow many randomized key locations should be used in this level (input 0 if none): ";
    cin >> keyAmount;
    cin.clear();
    cin.ignore(99, '\n');
    string levelName = "";
    cout << "\nPlease enter the name of your level (e.g. 'The Hallway' or 'Dark Cavern'): ";
    getline(cin, levelName);
    bool safetyCheck = false;
    string levelNumber;
    do {
        cout << "\nPlease enter the number your level should be saved as (e.g. 01, 02, 14, 28, etc.): ";
        cin >> levelNumber;
        //levelNumber.insert(0, "../"); // <-- not using this, but it would set the save location to parent folder of this project file
        levelNumber.insert(0, "GameData/Level"); // add level at the beginning of the filename
        levelNumber.insert(levelNumber.size(), ".txt"); // add .txt at the end of the filename
        ifstream levelTest;
        levelTest.open(levelNumber); // try to open existing file
        if (levelTest) { // then ask for confirmation if file exists
            hlp.SetColor(Color::Warning);
            cout << "\nWARNING: ";
            hlp.SetColor(Color::Default);
            cout << "File already exists, please confirm." << endl;
            cout << "  (1) Overwrite" << endl;
            cout << "  (2) Change Name" << endl;
            cout << "\nYour choice: ";
            int choice;
            cin >> choice;

            if (choice == 1) {
                safetyCheck = true;
            }
            else safetyCheck = false;
        }
        else safetyCheck = true; // always returns true if file doesn't exist
        levelTest.close(); // close the checked file
    } while (!safetyCheck);

    ofstream levelFile; // create the actual file
    levelFile.open(levelNumber); // set the name to the previously defined name
    if (!levelFile) {
        cout << "ERROR: Opening file failed!" << endl;
    }
    else { // write into the new file - so width, height and the full character array
        levelFile << width << endl; // simple values work similar to cout
        levelFile << height << endl;
        levelFile << keyAmount << endl;
        levelFile << levelName << endl;
        levelFile.write(pLevel, (long long)width * height); // whereas the syntax for the pointer array is a bit more involved, also the arithmetic overflow warning mentioned earlier would appear again without the type cast
        if (!levelFile) {
            cout << "ERROR: Write failed!" << endl;
        }
        levelFile.close(); // close and save the file to the harddrive
        cout << "\nLevel has been saved under [";
        hlp.SetColor(Color::Yellow);
        cout << levelNumber;
        hlp.SetColor(Color::Default);
        cout << "]. Press any key to return to the top menu..." << endl;
        char input = _getch();
    }
}

// this function works similar to the actual gameplay function
// cursor can be moved with the arrow keys and is limited to the user defined or loaded layout size from before
bool Editor::EditLevel(char* pLevel, int& cursorX, int& cursorY, int width, int height) {
    int newCursorX = cursorX;
    int newCursorY = cursorY;
    int userInput = _getch();

    if (userInput == Helpers::kArrowInput) {
        int arrowInput = _getch();
        switch (arrowInput) {
        case Helpers::kEscape:
            break;
        case Helpers::kLeftArrow:
            newCursorX--; // move cursor to the left
            break;
        case Helpers::kRightArrow:
            newCursorX++; // move cursor to the right
            break;
        case Helpers::kUpArrow:
            newCursorY--; // move cursor up
            break;
        case Helpers::kDownArrow:
            newCursorY++; // move cursor down
            break;
        }

        // check if cursor would be outside of the level bounds and clamp it
        if (newCursorX < 0) newCursorX = 0;
        else if (newCursorX == width) newCursorX = width - 1;
        if (newCursorY < 0) newCursorY = 0;
        else if (newCursorY == height) newCursorY = height - 1;

        cursorX = newCursorX;
        cursorY = newCursorY;
    }
    else {
        if (userInput == hlp.kEscape) { // pressing escape will set the return value to exit the editing loop - after confirmation
            int quitConfirmation = 0;
            cout << "\nAre you done editing this level?" << endl;
            cout << "  (1) Continue" << endl;
            cout << "  (2) Quit" << endl;
            cout << "\nYour choice: ";
            cin >> quitConfirmation;

            if (quitConfirmation == 2) {
                // finish editing
                g_LegendDisplay = false;
                return true;
            }
            else {
                // do nothing and go back to editing
                system("cls");
            }
        }
        else if (userInput == 8) {
            // do nothing - aka disallow the use of backspace as it would actually remove a character from the displayed array
        }
        else {
            // assign input character to appropriate location in the array
            int index = GetIndexFromXY(newCursorX, newCursorY, width);
            pLevel[index] = (char)userInput;
        }
    }
    return false;
}

// level dimension input from the user, don't allow any size below 5
void Editor::GetLevelDimensions(int& width, int& height) {
    cout << "Enter width of the level (min. 5): ";
    do {
        cin >> width;
        if (width < 5) cout << "The width needs to be 5 or higher, please try again: ";
    } while (width < 5);

    cout << "Enter height of the level (min. 5): ";
    do {
        cin >> height;
        if (height < 5) cout << "The height needs to be 5 or higher, please try again: ";
    } while (height < 5);
}

// re-draw the level after each edit or cursor movement
void Editor::DisplayLevel(char* pLevel, int width, int height, int cursorX, int cursorY, string levelName) {
    bool warningActive = false;
    int playerCount = 0;
    int exitCount = 0;
    hlp.ColoredText("Name: ", Color::Default);
    if (levelName != "") hlp.ColoredText(levelName, Color::Name);
    else hlp.ColoredText("NEW OR NOT SET", Color::Name);
    cout << endl;
    hlp.SetColor(Color::Default);
    DisplayTopBorder(width);
    for (int y = 0; y < height; y++) {
        DisplayLeftBorder();
        for (int x = 0; x < width; x++) {
            if (cursorX == x && cursorY == y) {
                cout << hlp.kCursor;
            }
            else { // set all valid level elements to their defined colors before drawing them
                int index = GetIndexFromXY(x, y, width);
                if (pLevel[index] == '@') {
                    hlp.SetColor(Color::Player);
                    playerCount++;
                }
                else if (pLevel[index] == 'R') hlp.SetColor(Color::Red);
                else if (pLevel[index] == 'G') hlp.SetColor(Color::Green);
                else if (pLevel[index] == 'B') hlp.SetColor(Color::Blue);
                else if (pLevel[index] == 'W') hlp.SetColor(Color::White);
                else if (pLevel[index] == 'r') hlp.SetColor(Color::Red);
                else if (pLevel[index] == 'g') hlp.SetColor(Color::Green);
                else if (pLevel[index] == 'b') hlp.SetColor(Color::Blue);
                else if (pLevel[index] == 'w') hlp.SetColor(Color::White);
                else if (pLevel[index] == '1') hlp.SetColor(Color::Red);
                else if (pLevel[index] == '2') hlp.SetColor(Color::Green);
                else if (pLevel[index] == '3') hlp.SetColor(Color::Blue);
                else if (pLevel[index] == '4') hlp.SetColor(Color::White);
                else if (pLevel[index] == 'X') {
                    hlp.SetColor(Color::Exit);
                    exitCount++;
                }
                else if (pLevel[index] == '+') hlp.SetColor(Color::Wall);
                else if (pLevel[index] == 'e') hlp.SetColor(Color::EnemyW);
                else if (pLevel[index] == 'E') hlp.SetColor(Color::EnemyS);
                else if (pLevel[index] == 'h') hlp.SetColor(Color::EnemyW);
                else if (pLevel[index] == 'H') hlp.SetColor(Color::EnemyS);
                else if (pLevel[index] == 'v') hlp.SetColor(Color::EnemyW);
                else if (pLevel[index] == 'V') hlp.SetColor(Color::EnemyS);
                else if (pLevel[index] == '$') hlp.SetColor(Color::Money);
                else if (pLevel[index] == '#') hlp.SetColor(Color::White);
                else if (pLevel[index] == '.') {
                    // do nothing, easteregg
                }
                else if (pLevel[index] == ' ') {
                    // do nothing for empty spaces
                }
                else {
                    // otherwise use a red warning color for illegal characters (and also set the warning message to true)
                    warningActive = true;
                    hlp.SetColor(Color::Warning);
                }
                cout << pLevel[index];
                hlp.SetColor(Color::Default);
            }
        }
        DisplayRightBorder();
    }
    DisplayBottomBorder(width);
    // displaying a little legend for the user, as well as the warning message if active:
    if (g_LegendDisplay) {
        hlp.ColoredText("\n\n @", Color::Player);
        hlp.ColoredText(" = Player Start Location", Color::Default);
        hlp.ColoredText("\n R", Color::Red);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("G", Color::Green);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("B", Color::Blue);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("W ", Color::White);
        hlp.ColoredText("= Colored Door", Color::Default);
        hlp.ColoredText("\n r", Color::Red);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("g", Color::Green);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("b", Color::Blue);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("w ", Color::White);
        hlp.ColoredText("= Colored Key", Color::Default);
        hlp.ColoredText("\n 1", Color::Red);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("2", Color::Green);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("3", Color::Blue);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("4", Color::White);
        hlp.ColoredText("= Unidentified Key of the respective color", Color::Default);
        hlp.ColoredText("\n X ", Color::Exit);
        hlp.ColoredText("= Exit", Color::Default);
        hlp.ColoredText("\n e", Color::EnemyW);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("E ", Color::EnemyS);
        hlp.ColoredText("= Weak/Strong Static Enemy", Color::Default);
        hlp.ColoredText("\n h", Color::EnemyW);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("H ", Color::EnemyS);
        hlp.ColoredText("= Weak/Strong Enemy with Horizontal Movement (", Color::Default);
        hlp.ColoredInt(hlp.kEnemyMoveHorW, Color::EnemyW);
        hlp.ColoredText(" or ", Color::Default);
        hlp.ColoredInt(hlp.kEnemyMoveHorS, Color::EnemyS);
        hlp.ColoredText(" tiles)", Color::Default);
        hlp.ColoredText("\n v", Color::EnemyW);
        hlp.ColoredText("/", Color::Default);
        hlp.ColoredText("V ", Color::EnemyS);
        hlp.ColoredText("= Weak/Strong Enemy with Vertical Movement (", Color::Default);
        hlp.ColoredInt(hlp.kEnemyMoveVerW, Color::EnemyW);
        hlp.ColoredText(" or ", Color::Default);
        hlp.ColoredInt(hlp.kEnemyMoveVerS, Color::EnemyS);
        hlp.ColoredText(" tiles)", Color::Default);
        hlp.ColoredText("\n $ ", Color::Money);
        hlp.ColoredText("= Collectible Money", Color::Default);
        hlp.ColoredText("\n + ", Color::Wall);
        hlp.ColoredText("= Wall", Color::Default);
        hlp.ColoredText("\n # ", Color::White);
        hlp.ColoredText("= White Key Spawner Randomizer Block", Color::Default);
        hlp.ColoredText("\n\nOnly the characters listed above are valid.", Color::Default);
        hlp.ColoredText("\nUse the ARROW keys to move the cursor. SPACE will clear a block. ESCAPE will exit the editor.\n", Color::Default);
    }
    if (playerCount == 0) {
        hlp.ColoredText("\nYOUR LEVEL NEEDS A PLAYER!", Color::Warning);
    }
    else if (playerCount > 1) {
        hlp.ColoredText("\nYOUR LEVEL HAS MORE THAN 1 PLAYER CHARACTERS!", Color::Warning);
    }
    else cout << "\n                                                         ";
    if (exitCount == 0) {
        hlp.ColoredText("\nYOUR LEVEL NEEDS AN EXIT!", Color::Warning);
    }
    else if (exitCount > 1) {
        hlp.ColoredText("\nYOUR LEVEL HAS MORE THAN 1 EXIT CHARACTERS!", Color::Warning);
    }
    else cout << "\n                                                         ";
    if (warningActive) {
        hlp.ColoredText("\nTHERE ARE UNUSED CHARACTERS IN YOUR LEVEL, PLEASE CHECK!", Color::Warning);
    } else cout << "\n                                                        ";
    hlp.SetColor(Color::Default);
}

int Editor::GetIndexFromXY(int x, int y, int width) {
    return x + y * width;
}

// functions to display the border limt around the actual editable level layout:
void Editor::DisplayTopBorder(int width) {
    cout << hlp.kTopLeftBorder;
    for (int i = 0; i < width; i++) {
        cout << hlp.kHorizontalBorder;
    }
    cout << hlp.kTopRightBorder << endl;
}

void Editor::DisplayBottomBorder(int width) {
    cout << hlp.kBottomLeftBorder;
    for (int i = 0; i < width; i++) {
        cout << hlp.kHorizontalBorder;
    }
    cout << hlp.kBottomRightBorder;
}

void Editor::DisplayLeftBorder() {
    cout << hlp.kVerticalBorder;
}

void Editor::DisplayRightBorder() {
    cout << hlp.kVerticalBorder << endl;
}