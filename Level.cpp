#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include <string>
#include <assert.h>

#include "Level.h"
#include "Player.h"
#include "Point.h"
#include "Key.h"
#include "Door.h"
#include "Exit.h"
#include "Enemy.h"
#include "Money.h"

using namespace std;

Level::Level()
    : m_pLevelData(nullptr)
    , m_width(0)
    , m_height(0)
    , m_finalLevel(4)
    , m_currentLevel(1)
    , m_activeLevel(0)
    , m_levelName("Unknown Location")
    , m_startLocationX(1)
    , m_startLocationY(1)
    , m_levelCollection(0)
    , m_quitToMenu(false)
{
}
Level::~Level() {
	if (m_pLevelData != nullptr) {
		delete[] m_pLevelData;
		m_pLevelData = nullptr;
	}

    while (!m_pActors.empty()) {
        delete m_pActors.back();
        m_pActors.pop_back();
    }
}

void Level::ResetLevelStates() {
    // clear the list of level files and reset back to level 1 (necessary in case a level was added in the editor or upon a restart in general)
    m_levelCollection.clear();
    m_currentLevel = 1;
    m_activeLevel = 0;
}

void Level::ResetCurrentLevel() {
    m_activeLevel--;
}

// check for existing level files in the same folder location (from 01 to 99) and add them to the level collection vector
void Level::FindLevels() {
    ResetLevelStates();
    ifstream levelFile;
    string levelNumber = "GameData/Level.txt";
    for (int level = 1; level < 100; level++) {
        levelNumber = "GameData/Level.txt";
        if (level < 10) {
            levelNumber.insert(14, "0");
            levelNumber.insert(15, to_string(level));
        }
        else levelNumber.insert(14, to_string(level));
        levelFile.open(levelNumber);
        if (!levelFile) {

        }
        else {
            m_levelCollection.push_back(levelNumber);
        }
        levelFile.close();
    }
    m_finalLevel = (int)m_levelCollection.size();
}

// fully dynamic function to load a new level from an existing text file, supports up to 99 levels for now
// general naming of the level files in the folder must be e.g. "Level01.txt" - "Level02.txt" - "Level06.txt" - "Level16.txt" - etc.
void Level::LoadLevel() {
    bool levelFound = false;
    // check if the level file can be found, otherwise skip to the next one in line and try again (loop until valid or m_finalLevel is reached)
    // technically this should never occur, unless the user deletes a level in the folder during gameplay
    do {
        ifstream levelFile;
        levelFile.open(m_levelCollection[(long long)m_currentLevel - 1]);
        if (!levelFile) {
            hlp.SetColor(Color::Warning);
            cout << "\n\nERROR: ";
            hlp.SetColor(Color::Default);
            cout << "File for 'Level";
            if (m_currentLevel < 10) cout << "0" << m_currentLevel;
            else cout << m_currentLevel;
            cout << "' not found or unable to open file. Press any key to try opening the next level...";
            if (_getch() && m_currentLevel < m_finalLevel) {
                m_currentLevel++;
                m_activeLevel++;
            }
            else {
                hlp.SetColor(Color::Warning);
                cout << "\n\nERROR: ";
                hlp.SetColor(Color::Default);
                cout << "No applicable level files found.\nPlease make sure that all 'LevelXX.txt' files are in the same folder as this application...\n";
                exit(0); // force quit the program
            }
        }
        else {
            levelFound = true;
            constexpr int tempSize = 99;
            char temp[tempSize];
            levelFile.getline(temp, tempSize, '\n'); // this will grab the number on the first line in the text file (assigned to width) - width of the level
            m_width = atoi(temp);
            levelFile.getline(temp, tempSize, '\n'); // second line in the text file (assigned to height) - height of the level
            m_height = atoi(temp);
            levelFile.getline(temp, tempSize, '\n'); // third line in the text file (assigned to keyAmount) - amount of random keys to spawn in the level
            int keyAmount = atoi(temp);
            levelFile.getline(temp, tempSize, '\n'); // this will grab the number on the fourth line in the text file
            m_levelName = temp;
            delete[] m_pLevelData; // unload current level array from memory
            m_pLevelData = new char[(long long)m_width * m_height]; // create a new pointer array for the level layout characters
            levelFile.read(m_pLevelData, long long(m_width) * m_height); // this will read all characters from the third line in the text file (position 0 to width * height) and write it to the level array
            // using a type cast above for width, otherwise it would throw a warning:
            // Warning C26451 Arithmetic overflow : Using operator '*' on a 4 byte value and then casting the result to a 8 byte value. Cast the value to the wider type before calling operator '*' to avoid overflow

            // randomize and add key locations based on the value imported from the level file on line 3, ignore function if value is 0
            if (keyAmount > 0) {
            RandomizeKeyLocation(keyAmount);
            }
        }
    } while (!levelFound);

    ConvertTiles(); // convert text file ascii characters to game ascii characters
    if (hlp.useExpWalls) WallConvert(); // experimental wall conversion
    // increase the active level number (to the current level value)
    m_activeLevel++;
    wallsDrawn = false; // reset bool to make sure level walls are drawn properly once
}

// spawn keys on randomized position into the level
void Level::RandomizeKeyLocation(int keyAmount) {
    bool validLocation = false;
    // for loop on how many keys should get spawned
    for (int i = 0; i < keyAmount; i++) {
        srand((unsigned)time(NULL));
        int tryCounter = 0;
        do {
            // randomize key position in the level, loop until it would be placed on a '#' tile
            int locX = rand() % m_width;
            int locY = rand() % m_height;
            int index = GetIndexFromCoordinates(locX, locY);
            tryCounter++;
            if (m_pLevelData[index] == '#') {
                validLocation = true;
                // also make sure white keys can't spawn next to each other (within a 6*6 area around an existing key)
                for (int a = -3; a < 4; a++) {
                    for (int b = -3; b < 4; b++) {
                        if (m_pLevelData[index + a + (b * m_width)] == 'w') {
                            validLocation = false;
                        }
                    }
                }
                if (tryCounter > 9999) validLocation = true; // safety counter to ignore spread out placement rule to avoid an endless loop (might happen if you try to place too many keys in a small level)
                if (validLocation) m_pLevelData[index] = 'w';
            }
        } while (!validLocation);
        validLocation = false;
    }
    // replace all key spawner blocks with empty spaces
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            int index = GetIndexFromCoordinates(x, y);
            if (m_pLevelData[index] == '#') {
                m_pLevelData[index] = ' ';
            }
        }
        cout << endl;
    }
}

// increase the number of the level the player should be on
void Level::IncreaseLevel() {
    m_currentLevel++;
}

// increase the number of the currently loaded level
void Level::IncreaseActiveLevel() {
    m_activeLevel++;
}

// draw all mapped characters from the 1-dimensional array based on the x * y grid layout
void Level::DrawMap() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (!wallsDrawn) { // only draw level layout once (to prevent flickering of actors on each update)
        for (int y = 0; y < GetHeight(); y++) {
            cout << " "; // additional space at the beginning on each line to move the entire layout 1 spot to the right - just optics
            for (int x = 0; x < GetWidth(); x++) {
                int index = GetIndexFromCoordinates(x, y);
                hlp.ColoredChar(m_pLevelData[index], Color::Wall); // draw wall tiles from the layout array
            }
            cout << endl; // linebreak for each width "end"
        }
        wallsDrawn = true;
    }

    COORD actorPosition;
    // draw everything in the actors vector with their appropriate data
    for (auto actor = m_pActors.begin(); actor != m_pActors.end(); actor++) {
        if ((*actor)->IsActive()) {
            actorPosition.X = (*actor)->GetPositionX() + hlp.kOffsetX; // +minor X adjustment for the actual map layout position
            actorPosition.Y = (*actor)->GetPositionY() + hlp.kOffsetY; // +minor Y adjustment for the inventory above the layout
            SetConsoleCursorPosition(console, actorPosition);
            (*actor)->Draw();
        }
    }
}

Actor* Level::UpdateActors(int x, int y) {
    Actor* collidedActor = nullptr;
    for (auto actor = m_pActors.begin(); actor != m_pActors.end(); actor++) {
        (*actor)->Update();

        if (x == (*actor)->GetPositionX() && y == (*actor)->GetPositionY()) {
            //assert(collidedActor == nullptr); <-- disabling this assert to avoid crash when play would collide with two different enemies at the same location
            collidedActor = (*actor);
        }
    }
    return collidedActor;
}

// check if this tile on the character array is empty
bool Level::IsSpace(int x, int y) {
    return m_pLevelData[GetIndexFromCoordinates(x, y)] == ' ';
}

// check if this tile on the character array is the easteregg item
bool Level::IsEasteregg(int x, int y) {
    return m_pLevelData[GetIndexFromCoordinates(x, y)] == '.';
}

bool Level::IsWall(int x, int y) {
    char tile = m_pLevelData[GetIndexFromCoordinates(x, y)];
    bool hitWall = hlp.WallHitCheck(tile);
    return hitWall;
}

// set this specific coordinate in the character array to empty (space)
void Level::ClearTile(int x, int y) {
    m_pLevelData[GetIndexFromCoordinates(x, y)] = ' ';
}

// convert all characters from the loaded text array into their actual character ascii used for the game
void Level::ConvertTiles() {
    m_pActors.clear();
    srand((unsigned)time(NULL));
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            int index = GetIndexFromCoordinates(x, y);
            switch (m_pLevelData[index]) {
            // wall:
            case '+':
                m_pLevelData[index] = hlp.WAL;
                break;
            // player:
            case '@':
                m_pLevelData[index] = ' ';
                //m_pActors.push_back(playerData(x, y, playerData.GetColor()));
                m_startLocationX = x;
                m_startLocationY = y;
                break;
            // keys:
            case 'r':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Key(x, y, Color::Red, false));
                break;
            case 'g':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Key(x, y, Color::Green, false));
                break;
            case 'b':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Key(x, y, Color::Blue, false));
                break;
            case 'w':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Key(x, y, Color::White, false));
                break;
            // unidentified keys:
            case '1':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Key(x, y, Color::Red, true));
                break;
            case '2':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Key(x, y, Color::Green, true));
                break;
            case '3':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Key(x, y, Color::Blue, true));
                break;
            case '4':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Key(x, y, Color::White, true));
                break;
            // exit:
            case 'X':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Exit(x, y, Color::Exit));
                break;
            // doors:
            case 'R':
                m_pLevelData[index] = hlp.DOR;
                m_pActors.push_back(new Door(x, y, Color::Red));
                break;
            case 'G':
                m_pLevelData[index] = hlp.DOR;
                m_pActors.push_back(new Door(x, y, Color::Green));
                break;
            case 'B':
                m_pLevelData[index] = hlp.DOR;
                m_pActors.push_back(new Door(x, y, Color::Blue));
                break;
            case 'W':
                m_pLevelData[index] = hlp.DOR;
                m_pActors.push_back(new Door(x, y, Color::White));
                break;
            // enemies
            case 'e':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Enemy(x, y, Color::EnemyW));
                break;
            case 'E':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Enemy(x, y, Color::EnemyS));
                break;
            case 'h':
                m_pLevelData[index] = ' '; // 1st random number: movement start direction, 2nd: movement start delay, 3rd: movement speed
                m_pActors.push_back(new Enemy(x, y, Color::EnemyW, hlp.kEnemyMoveHorW, 0, rand() % 2, rand() % 4, hlp.kEnemyMoveSpeed - rand() % hlp.kEnemyMoveModW));
                break;
            case 'H':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Enemy(x, y, Color::EnemyS, hlp.kEnemyMoveHorS, 0, rand() % 2, rand() % 4, hlp.kEnemyMoveSpeed - rand() % hlp.kEnemyMoveModS));
                break;
            case 'v':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Enemy(x, y, Color::EnemyW, 0, hlp.kEnemyMoveVerW, rand() % 2, rand() % 4, hlp.kEnemyMoveSpeed - rand() % hlp.kEnemyMoveModW));
                break;
            case 'V':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Enemy(x, y, Color::EnemyS, 0, hlp.kEnemyMoveVerS, rand() % 2, rand() % 4, hlp.kEnemyMoveSpeed - rand() % hlp.kEnemyMoveModS));
                break;
            // extra stuff:
            case '$':
                m_pLevelData[index] = ' ';
                m_pActors.push_back(new Money(x, y, Color::Money, 1 + rand() % 5));
                break;
            case '.':
                // Easteregg, do nothing
                break;
            default: // remove all remaining unused or illegal characters, if there are any
                m_pLevelData[index] = ' ';
                break;
            }
        }
    }
}

// trying another approach to dynamically draw walls based on their position and neighbors (actual walls with corners and connectors instead of full blocks)
void Level::WallConvert() {
    char* tempWalls = new char[(long long)m_width * m_height]; // creating a temporary array to store the new wall characters (overwriting in the actual level array at this point would cause issues during the check loop)
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            int index = GetIndexFromCoordinates(x, y);
            int wallCounter = 0;
            if (m_pLevelData[index] == hlp.WAL) {
                // check if there are any walls or doors next to this iteration of the current index
                if (x > 0 && (m_pLevelData[index - 1] == hlp.WAL || m_pLevelData[index - 1] == hlp.DOR)) {
                    wallCounter += 1; // wall found to the left
                }
                if (x < m_width - 1 && (m_pLevelData[index + 1] == hlp.WAL || m_pLevelData[index + 1] == hlp.DOR)) {
                    wallCounter += 2; // wall found to the right
                }
                if (y > 0 && (m_pLevelData[index - m_width] == hlp.WAL || m_pLevelData[index - m_width] == hlp.DOR)) {
                    wallCounter += 4; // wall found to the top
                }
                if (y < m_height - 1 && (m_pLevelData[index + m_width] == hlp.WAL || m_pLevelData[index + m_width] == hlp.DOR)) {
                    wallCounter += 8; // wall found to the bottom
                }
                // based on the sum of wallCounter, draw the appropriate wall character:
                switch (wallCounter) {
                case 0: // no connecting wall tile
                    tempWalls[index] = hlp.WSNG; // 250
                    break;
                case 1: // only 1 wall to the left
                    tempWalls[index] = hlp.WHRZ; // 196
                    break;
                case 2: // only 1 wall to the right
                    tempWalls[index] = hlp.WHRZ; // 196
                    break;
                case 3: // 1 wall left, 1 wall right
                    tempWalls[index] = hlp.WHRZ; // 196
                    break;
                case 4: // only 1 wall to the top
                    tempWalls[index] = hlp.WVRT; // 179
                    break;
                case 5: // 1 wall left, 1 wall top
                    tempWalls[index] = hlp.WCBR; // 217
                    break;
                case 6: // 1 wall right, 1 wall top
                    tempWalls[index] = hlp.WCBL; // 192
                    break;
                case 7: // 1 wall left, 1 wall right, 1 wall top
                    tempWalls[index] = hlp.WTRU; // 193
                    break;
                case 8: // only 1 wall to the bottom
                    tempWalls[index] = hlp.WVRT; // 179
                    break;
                case 9: // 1 wall left, 1 wall bottom
                    tempWalls[index] = hlp.WCTR; // 191
                    break;
                case 10: // 1 wall right, 1 wall bottom
                    tempWalls[index] = hlp.WCTL; // 218
                    break;
                case 11: // 1 wall left, 1 wall right, 1 wall bottom
                    tempWalls[index] = hlp.WTRD; // 194
                    break;
                case 12: // 1 wall top, 1 wall bottom
                    tempWalls[index] = hlp.WVRT; // 179
                    break;
                case 13: // 1 wall left, 1 wall top, 1 wall bottom
                    tempWalls[index] = hlp.WTRL; // 180
                    break;
                case 14: // 1 wall right, 1 wall top, 1 wall bottom
                    tempWalls[index] = hlp.WTRR; // 195
                    break;
                case 15: // all directions
                    tempWalls[index] = hlp.WCRO; // 197
                    break;
                default:
                    tempWalls[index] = hlp.WAL; // putting this here to avoid the uninitialized memory warning
                    break;
                }
            }
        }
    }
    // replace all wall blocks in the actual level array with the new ones from the temporary array
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            int index = GetIndexFromCoordinates(x, y);
            if (m_pLevelData[index] == hlp.WAL) {
                m_pLevelData[index] = tempWalls[index];
            }
            if (m_pLevelData[index] == hlp.DOR) {
                m_pLevelData[index] = ' ';
            }
        }
        cout << endl;
    }
    delete[] tempWalls; // delete the temporary array from memory
}

// return the index number in the character array based on the defined x/y/width coordinates
int Level::GetIndexFromCoordinates(int x, int y) {
    return x + (y * m_width);
}