#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "Interface.h"
#include "Helpers.h"
#include "Input.h"

using namespace std;

Interface::Interface() {
}
Interface::~Interface() {
}

int Interface::WelcomeScreen(int finalLevel) {
    hlp.SetColor(Color::Exit);
    char welcomeScreen[] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                             ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', hlp.WAL,
                             ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL,
                             ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL,
                             ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL,
                             ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', hlp.WAL,
                             ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', hlp.WAL,
                             ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL,
                             ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ',
                             ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL,
                             ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', hlp.WAL,
                             ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

    for (int y = 0; y < 12; y++) {
        for (int x = 0; x < 36; x++) {
            if ((x > 7 && x < 11) || (x > 15 && x < 19) || (x > 23 && x < 29) || (x > 33)) {
                hlp.SetColor(Color::Money);
            }
            else hlp.SetColor(Color::Exit);
            cout << welcomeScreen[x + (y * 36)];
        }
        cout << endl;
    }
    hlp.ColoredText("\n Oh, hello! The player is trapped in a dungeon. Your goal is to reach the exit (", Color::Default);
    hlp.ColoredChar(hlp.EXT, Color::Exit);
    hlp.ColoredText(") on floor ", Color::Default);
    hlp.ColoredInt(finalLevel, Color::Exit);
    hlp.ColoredText(".\n\n Locked colored doors (", Color::Default);
    hlp.ColoredChar(hlp.DOR, Color::Red);
    hlp.ColoredChar(hlp.DOR, Color::Green);
    hlp.ColoredChar(hlp.DOR, Color::Blue);
    hlp.ColoredChar(hlp.DOR, Color::White);
    hlp.ColoredText(") will hinder you on your journey and require matching colored keys (", Color::Default);
    hlp.ColoredChar(hlp.KEY, Color::Red);
    hlp.ColoredChar(hlp.KEY, Color::Green);
    hlp.ColoredChar(hlp.KEY, Color::Blue);
    hlp.ColoredChar(hlp.KEY, Color::White);
    hlp.ColoredText(") to be opened.\n\n", Color::Default);
    hlp.ColoredText(" Be warned that you might stumble upon an unidentified ", Color::Default);
    hlp.ColoredChar(hlp.KEY, Color::Turquoise);
    hlp.ColoredText(" key. These won't ever reveal their true color until you open the correct door.", Color::Default);
    hlp.ColoredText("\n\n Your player character (", Color::Default);
    hlp.ColoredChar(hlp.kPlayer, Color::Player);
    hlp.ColoredText(") can be moved with either ", Color::Default);
    hlp.ColoredText("W-A-S-D", Color::White);
    hlp.ColoredText(" or the ", Color::Default);
    hlp.ColoredText("ARROW", Color::White);
    hlp.ColoredText(" keys.", Color::Default);
    hlp.ColoredText("\n\n Heck, there are even rumors of hidden treasures (", Color::Default);
    hlp.ColoredChar(hlp.MON, Color::Money);
    hlp.ColoredText(") scattered about, but they might be guarded by some enemies (", Color::Default);
    hlp.ColoredChar(hlp.EMY, Color::EnemyS);
    hlp.ColoredText(").\n\n Remember, you are not invincible! Anyway, GOOD LUCK!\n\n\n Press '", Color::Default);
    hlp.ColoredText("S", Color::Green);
    hlp.ColoredText("' to start, '", Color::Default);
    hlp.ColoredText("E", Color::Blue);
    hlp.ColoredText("' to launch the editor or '", Color::Default);
    hlp.ColoredText("Q", Color::Red);
    hlp.ColoredText("' to quit...", Color::Default);
    while (1) {
        char input = _getch();
        if (in.KeyDown(in.kKeyboardE)) {
            return 2;
        }
        else if (in.KeyDown(in.kKeyboardQ)) {
            return 3;
        }
        else if (finalLevel == 0) { // if this is 0, no valid level files could be found, which in turn disallows to start the game and displays a warning
            return 4;
        }
        else if (in.KeyDown(in.kMoveDown)) { // S
            return 1; // otherwise start the game normally
        }
    }
}

// little inventory, level name, coordinate and floor info displayed above the level layout
void Interface::Inventory(int currentLevel, int pPosX, int pPosY, int* keys, bool easteregg, std::string levelName, int health, int money) {
    hlp.DisplayCursor(false); // refreshing cursor hide in case screen gets resized during gameplay
    hlp.SetColor(Color::Default);
    if (health == 100) {
        cout << "HEALTH: ";
        hlp.SetColor(Color::Green);
        cout << health;
    }
    else if (health < 100 && health > 30) {
        cout << "HEALTH: ";
        hlp.SetColor(Color::Exit);
        cout << "0" << health;
    }
    else {
        cout << "HEALTH: ";
        hlp.SetColor(Color::Red);
        cout << "0" << health;
    }
    hlp.SetColor(Color::Default);
    cout << " || CURRENT FLOOR: ";
    hlp.SetColor(Color::Exit);
    if (currentLevel < 10) cout << "0" << currentLevel;
    else cout << currentLevel;
    hlp.SetColor(Color::Default);
    cout << " || Player Position: X";
    if (pPosX < 10) cout << "0" << pPosX;
    else cout << pPosX;
    cout << ", Y";
    if (pPosY < 10) cout << "0" << pPosY << endl;
    else cout << pPosY << endl;
    cout << "INVENTORY: ";

    hlp.ColoredChar(hlp.KEY, Color::Red);
    hlp.SetColor(Color::Default);
    cout << "x" << keys[0] << "/";
    hlp.ColoredChar(hlp.KEY, Color::Green);
    hlp.SetColor(Color::Default);
    cout << "x" << keys[1] << "/";
    hlp.ColoredChar(hlp.KEY, Color::Blue);
    hlp.SetColor(Color::Default);
    cout << "x" << keys[2] << "/";
    hlp.ColoredChar(hlp.KEY, Color::White);
    hlp.SetColor(Color::Default);
    cout << "x" << keys[3] << "/";
    hlp.ColoredChar(hlp.KEY, Color::Turquoise);
    hlp.SetColor(Color::Default);
    cout << "x" << keys[4] + keys[5] + keys[6] + keys[7];

    hlp.SetColor(Color::Default);
    cout << " || GOLD: " << money;
    
    if (easteregg) cout << " || 1 x Useless Junk?!";
    cout << "\n" << endl;
    hlp.SetColor(Color::Name);
    if (levelName != "") cout << " >> " << levelName << endl;
    else cout << " >> Unknown Location" << endl; // default name in case a level has not been named
}

// small colored legend and an additional hint when trying to open a door without a key
void Interface::Legend(bool keyWarning) {
    if (keyWarning) {
        hlp.ColoredText("\nThe door is locked tight. Find a key with the matching color!", Color::Warning);
    } else cout << "\n                                                                                        ";
    hlp.ColoredText("\n\n LEGEND:\n ", Color::Default);
    
    
    hlp.ColoredChar(hlp.kPlayer, Color::Player);
    hlp.ColoredText(" -> Player\n ", Color::Default);
    hlp.ColoredChar(hlp.EXT, Color::Exit);
    hlp.ColoredText(" -> Exit\n ", Color::Default);
    hlp.ColoredChar(hlp.KEY, Color::White);
    hlp.ColoredText(" / ", Color::Default);
    hlp.ColoredChar(hlp.DOR, Color::White);
    hlp.ColoredText(" -> Key & Door\n ", Color::Default);
    hlp.ColoredChar(hlp.EMY, Color::EnemyS);
    hlp.ColoredText(" -> Enemy\n ", Color::Default);
    hlp.ColoredChar(hlp.MON, Color::Money);
    hlp.ColoredText(" -> Gold", Color::Default);
    hlp.ColoredText("\n\n Press 'ESCAPE' to return to the main menu (progress is lost)", Color::Default);
    hlp.ColoredText("\n 'R' will reset the current level (if you are stuck)", Color::Default);
}

void Interface::GameOverScreen(bool easteregg, int money, int health) {
    system("cls");
    cout << endl;
    char gameOverScreen[] = { ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                              ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ',
                              ' ', ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ',
                              ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ',
                              ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ',
                              ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ',
                              ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ',
                              ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ',
                              ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ',
                              ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ', ' ',
                              ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ',
                              ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', hlp.WAL, hlp.WAL, ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

    for (int y = 0; y < 12; y++) {
        for (int x = 0; x < 34; x++) {
            if (x > 20) {
                if (easteregg && health == 100) hlp.SetColor(Color::Warning); // only show the heart if you picked up the easteregg item and have max health :)
                else hlp.SetColor(Color::Black);
            }
            else hlp.SetColor(Color::Exit);
            cout << gameOverScreen[x + (y * 34)];
        }
        cout << endl;
    }
    hlp.ColoredText("\n\n Good job! Unfortunately, the princess is in another castle...\n", Color::Default);
    if (money > 0) {
        hlp.ColoredText("\n At least you collected ", Color::Default);
        hlp.ColoredInt(money, Color::Money);
        hlp.ColoredText(" coins of ", Color::Default);
        hlp.ColoredChar(hlp.MON, Color::Money);
        hlp.ColoredText(". That's something, right?\n", Color::Default);
    }
    else {
        hlp.ColoredText("\n How in the world did you manage to not collect a single ", Color::Default);
        hlp.ColoredChar(hlp.MON, Color::Money);
        hlp.ColoredText(" on your way here?\n", Color::Default);
    }
    if (easteregg && health == 100) { // only show this if you picked up the easteregg item and have max health
        cout << "\n In any case, you get a big heart for finding the super hidden secret item while not losing a single point of health! ";
        hlp.ColoredText("<3 \n", Color::Red);
    }
    hlp.SoundPlayer(4);
    hlp.ColoredText("\n\n Press 'ESCAPE' to return to the main menu...\n", Color::Default);
    hlp.WaitForEscape();
}

void Interface::DeathScreen() {
    system("cls");
    cout << endl;
    char gameOverScreen[] = { ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                              ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ',
                              ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ',
                              ' ', ' ', ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ',
                              ' ', ' ', ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, hlp.WAL, ' ',
                              ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ',
                              ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ',
                              ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ',
                              ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ',
                              ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ',
                              ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ', ' ', hlp.WAL, ' ', ' ', hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', hlp.WAL, ' ', hlp.WAL, ' ', hlp.WAL, ' ', ' ', ' ',
                              ' ', ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, hlp.WAL, ' ', hlp.WAL, hlp.WAL, hlp.WAL, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

    for (int y = 0; y < 12; y++) {
        for (int x = 0; x < 34; x++) {
            if (x > 20) {
                hlp.SetColor(Color::Default);
            }
            else hlp.SetColor(Color::Red);
            cout << gameOverScreen[x + (y * 34)];
        }
        cout << endl;
    }
    hlp.ColoredText("\n\n Well, that didn't go as planned. Be more careful next time and don't forget to pick up your souls!\n", Color::Default);
    hlp.SoundPlayer(7);
    cout << "\n\n Press 'ESCAPE' to return to the main menu..." << endl;
    hlp.WaitForEscape();
}