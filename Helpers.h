#pragma once
#include "Colors.h"

static class Helpers {
public:
	Helpers();
	~Helpers();

	void SetColor(Color colorId);
	void ColoredText(std::string text, Color colorId);
	void ColoredChar(char chr, Color colorId);
	void ColoredInt(int number, Color colorId);

	// both beep functions need to be static, otherwise "thread" would not work here:
	static void SoundPlayer(int trackId);
	static void SoundTable(int trackId);
	void ClearScreen();
	void DisplayCursor(bool showCursor);
	void ClearCursorPosition(short x, short y);
	char ReadOutput(short x, short y);
	void WaitForEscape();
	bool WallHitCheck(char tile);

	// offsets for setting the correct console cursor location in the level layout area
	const short kOffsetX = 1;
	const short kOffsetY = 4;

	// game speed modifier
	const int kGameSpeed = 15;

	// player
	static const char kPlayer = 64; // player character symbol
	const int kPlayerStartingHealth = 100;

	// enemy colors & values
	const char EMY = 15; // enemy character symbol || -49 alternative
	const int kEnemyWeak = 10; // damage value
	const int kEnemyStrong = 20; // damage value
	const int kEnemyMoveHorW = 2; // horizontal movement range of weak enemies
	const int kEnemyMoveHorS = 4; // horizontal movement range of strong enemies
	const int kEnemyMoveVerW = 1; // vertical movement range of weak enemies
	const int kEnemyMoveVerS = 2; // vertical movement range of strong enemies
	// artificial delay counter for slower real time enemy movements
	const int kEnemyMoveSpeed = 12;
	const int kEnemyMoveModW = 2;
	const int kEnemyMoveModS = 7;

	// actor characters
	const char KEY = -66; // character used for keys
	const char DOR = -78; // character used for doors
	const char DOO = -80; // character used for open doors
	const char EXT = -99; // character used for the exit
	const char MON = '$'; // character used for money/gold

	// default and fallback full block wall
	const char WAL = -37; 
	// set to false to ignore the exp conversion code and draw the full block walls only:
	const bool useExpWalls = true;
	// experimental walls
	const char WSNG = -6; // single wall without connections
	const char WHRZ = -60; // horizontal wall
	const char WVRT = -77; // vertical wall
	const char WCTL = -38; // top left corner
	const char WCTR = -65; // top right corner
	const char WCBL = -64; // bottom left corner
	const char WCBR = -39; // bottom right corner
	const char WTRU = -63; // triple connector up/L/R
	const char WTRD = -62; // triple connector down/L/R
	const char WTRL = -76; // triple connector left/U/D
	const char WTRR = -61; // triple connector right/U/D
	const char WCRO = -59; // wall cross

	// characters for the border drawn around the editable area in the editor
	const char kTopRightBorder = -69;
	const char kTopLeftBorder = -55;
	const char kBottomRightBorder = -68;
	const char kBottomLeftBorder = -56;
	const char kHorizontalBorder = -51;
	const char kVerticalBorder = -70;

	// arrow keys (need to be static in order to use them in switches):
	static const int kArrowInput = 224;
	static const int kLeftArrow = 75;
	static const int kRightArrow = 77;
	static const int kUpArrow = 72;
	static const int kDownArrow = 80;

	// user cursor in the editor
	const char kCursor = '_';

	// escape and reset keys:
	static const int kEscape = 27;
	const char kReset = 0x52; // R
} hlp;