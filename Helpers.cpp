#pragma comment(lib, "winmm.lib") // windows multimedia library, required to play .wav files
#include <iostream>
#include <thread>
#include <Windows.h>
#include <conio.h>
#include "Helpers.h"

using namespace std;

Helpers::Helpers()
{
}
Helpers::~Helpers() {
}

// intermediary function for SetColor to keep the call in a single line for simple text outputs
void Helpers::ColoredText(string text, Color colorId) {
    SetColor(colorId);
    cout << text;
}

// intermediary function for SetColor to keep the call in a single line for simple character outputs
void Helpers::ColoredChar(char chr, Color colorId) {
    SetColor(colorId);
    cout << chr;
}

// intermediary function for SetColor to keep the call in a single line for simple number outputs
void Helpers::ColoredInt(int number, Color colorId) {
    SetColor(colorId);
    cout << number;
}

// using a custom function to change text/character colors with way less syntax, also avoids having to use the HANDLE call in every function using colors
void Helpers::SetColor(Color colorId) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, (int)colorId);
}

// intermediary function to make the sound calls in an extra thread (async from the game loop, so no pause)
void Helpers::SoundPlayer(int trackId) {
    thread soundFunction(SoundTable, trackId);
    soundFunction.detach();
}

// custom sound table with pre-defined "tracks" that can be called based on their number
// EXPERIMENTAL: using PlaySound with external .wav files instead of beeps
void Helpers::SoundTable(int trackId) {
    switch (trackId) {
        case 0: { // game start
            //Beep(659, 125);
            //Sleep(50);
            //Beep(659, 125);
            //Sleep(125);
            //Beep(659, 275);
            //Sleep(125);
            //Beep(523, 125);
            //Sleep(50);
            //Beep(659, 125);
            //Sleep(125);
            //Beep(784, 225);
            PlaySound(TEXT("GameData/Sound/snd_start.wav"), NULL, SND_FILENAME | SND_SYNC); // sync, can't be overwritten by another PlaySound
            break;
        }
        case 1: { // key pickup
            //Beep(550, 125);
            //Beep(700, 125);
            //Beep(850, 150);
            PlaySound(TEXT("GameData/Sound/snd_keyPickup.wav"), NULL, SND_FILENAME | SND_ASYNC); // async, can be overwritten by a newer PlaySound
            break;
        }
        case 2: { // door opened
            //Beep(300, 125);
            //Beep(450, 200);
            PlaySound(TEXT("GameData/Sound/snd_openDoor.wav"), NULL, SND_FILENAME | SND_ASYNC);
            break;
        }
        case 3: { // door locked
            //Beep(230, 150);
            //Beep(200, 400);
            PlaySound(TEXT("GameData/Sound/snd_lockedDoor.wav"), NULL, SND_FILENAME | SND_ASYNC);
            break;
        }
        case 4: { // game end
            //Beep(698, 150);
            //Beep(698, 150);
            //Sleep(100);
            //Beep(587, 150);
            //Beep(587, 150);
            //Sleep(100);
            //Beep(622, 150);
            //Beep(622, 150);
            PlaySound(TEXT("GameData/Sound/snd_win.wav"), NULL, SND_FILENAME | SND_ASYNC);
            break;
        }
        case 5: { // level change
            //Beep(350, 125);
            //Beep(250, 125);
            //Beep(150, 300);
            PlaySound(TEXT("GameData/Sound/snd_levelChange.wav"), NULL, SND_FILENAME | SND_ASYNC);
            break;
        }
        case 6: { // enemy hit sound
            //Beep(150, 400);
            PlaySound(TEXT("GameData/Sound/snd_enemyHit.wav"), NULL, SND_FILENAME | SND_ASYNC);
            break;
        }
        case 7: { // death
            //Beep(150, 300);
            //Beep(100, 800);
            PlaySound(TEXT("GameData/Sound/snd_death.wav"), NULL, SND_FILENAME | SND_ASYNC);
            break;
        }
        case 8: { // money
            //Beep(950, 200);
            PlaySound(TEXT("GameData/Sound/snd_money.wav"), NULL, SND_FILENAME | SND_ASYNC);
            break;
        }
    }
}

// custom function to show/hide the blinking _ cursor
// has the neat side effect of also having it not flicker across the level layout when running the update loop
void Helpers::DisplayCursor(bool showCursor) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO  ConsoleCursorInfo;
    ConsoleCursorInfo.dwSize = 1; // including this seems to be essential for the cursorinfo to work properly
    if (showCursor) {
        ConsoleCursorInfo.bVisible = true;
    } else ConsoleCursorInfo.bVisible = false;
    SetConsoleCursorInfo(console, &ConsoleCursorInfo);
}

// custom function to "clear" the screen during gameplay
// essentially just sets the cursor back to 0,0 and allows to re-draw (overwrite) everything at the same screen positions, but avoids the constant flickering from cls and is faster
void Helpers::ClearScreen() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(console, coord); // set cursor postion to x0, y0 on the screen
}

void Helpers::ClearCursorPosition(short x, short y) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { x, y };
    SetConsoleCursorPosition(console, coord); // set cursor postion to x, y on the screen
    cout << ' ';
}

// read and return character on specific console output coordinate
char Helpers::ReadOutput(short x, short y) {
    char ch;
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &screenBufferInfo);
    char buf[2]; COORD c = { x, y }; DWORD num_read;
    ReadConsoleOutputCharacter(console, (LPTSTR)buf, 1, c, (LPDWORD)&num_read);
    return ch = buf[0];
}

void Helpers::WaitForEscape() {
    bool notEscape = false;
    do {
        char input = _getch();
        if (input == kEscape) notEscape = true;
    } while (!notEscape);
}

bool Helpers::WallHitCheck(char tile) {
    bool isWall = (tile == WAL || tile == WSNG || tile == WHRZ || tile == WVRT || tile == WCTL || tile == WCTR
        || tile == WCBL || tile == WCBR || tile == WTRU || tile == WTRD || tile == WTRL || tile == WTRR || tile == WCRO);
    return isWall;
}