#include <iostream>
#include <conio.h>
#include "Game.h"
#include "Editor.h"

// initialize the game files
Game* myGame = new Game;
Editor myEditor;

int main() {
	int selection;
	do {
		hlp.DisplayCursor(false); // hide the blinking cursor (will show up again if the screen gets resized, though, so putting this inside the loop to refresh)
		myGame->m_level.FindLevels(); // try to find valid level files dynamically and store their names, also set the final level based on the amount
		system("cls"); // clear screen
		selection = myGame->m_interface.WelcomeScreen(myGame->m_level.GetFinalLevel()); // display an introductory how-to-play message
		switch (selection) {
		case 1: // 's' was pressed on the welcome screen
			hlp.SoundPlayer(0); // adding some starting sounds (async from the rest of the program)
			myGame->Run(); // load the game
			break;
		case 2: // 'e' was pressed on the welcome screen
			myEditor.EditorMain(); // load the editor
			break;
		case 3: // 'q' was pressed on the welcome screen
			// quit the application
			break;
		case 4: // automatically set if game couldn't find any level data only before going into case 1, won't show for cases 2/3
			system("cls");
			hlp.ColoredText("\nWARNING: No valid level files in folder found!", Color::Warning);
			hlp.ColoredText("\n\nPlease make sure all 'LevelXX.txt' files are in the same folder as this application.", Color::Default);
			hlp.ColoredText("\nYou can also use the integrated level editor to create your own levels.", Color::Default);
			hlp.ColoredText("\n\nPress any key to return to the main menu...", Color::Default);
			char input = _getch();
			break;
		}
	} while (selection != 3);
	std::cout << std::endl;

	if (myGame != nullptr) {
		delete myGame;
		myGame = nullptr;
	}
}