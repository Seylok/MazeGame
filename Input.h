#pragma once
#include <Windows.h>

static struct Input {
	bool KeyDown(char chr) {
		if (GetAsyncKeyState(chr) < 0) return true;
		else return false;
	}
	bool keyPressedUp = false;
	bool keyPressedDown = false;
	bool keyPressedLeft = false;
	bool keyPressedRight = false;
	
	// timers (counters) for automatic movement when holding down keys
	int keyHeldTimer = 0;
	const int keyHeldTimerMax = 25;

	// movement keys for the player during the game
	const char kMoveUp = 0x57; // W
	const char kMoveDown = 0x53; // S
	const char kMoveLeft = 0x41; // A
	const char kMoveRight = 0x44; // D

	// additional keys for the main menu
	const char kKeyboardE = 0x45; // E
	const char kKeyboardQ = 0x51; // Q
} in;