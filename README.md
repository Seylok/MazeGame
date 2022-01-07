# MazeGame

This is a small windows console gaming project I worked on during my 3 months long C++ introductory course with CGSpectrum.
I had some hobbyist experience with C# in Unity before, but this is my first actual foray into C++ and GitHub as well.

Since the course material was a little too basic (in terms of gameplay and optics) for my personal taste, I added quite a lot of additional mechanics and quality of life features:

* Realtime gameplay/movement (the course version was purely update-per-keypress)
* Enhanced the screen updating methods to a custom one to avoid any kind of flickering (so no CLS during gameplay, though still not actual buffering)
* Level Editor
  - Enhanced set of features such as file detection, proper loading/saving and naming
  - Full coloration and legend of all important characters
  - Some safety features when working on and saving existing files
  - Directly included into the game and can be launched from the main menu (newly created levels will also be detected and used immediately)
* Automatic detection for existing and user created levels at game start
* Proper windows folder usage for all external file (levels, sounds)
* A custom and fully dynamic "wall" conversion system based on the amount of connected wall tiles (course version only used full block ascii)
* Enemies
  - Randomized most values to make them more dynamic
  - Meaning their starting directions, movement speeds and start delays change with every level start or reset
  - Custom obstacle detection based on reading the current console window so they never actually run into walls or other objects
* Keys
  - A proper inventory system reflected on the interface for all key types (course version only allowed for one key at a time)
  - A new specific key-type that allows for random spawn locations
  - All keys can also be placed as "unidentified" (meaning the color is unknown until a door with the correct color is opened)
* Usage of external .wav files for all sounds in the game (yes, I know, these are ALTTP sounds :)) instead of "beeps"
* Sounds will play asychronous and not freeze the game while they play (course version beeps do exactly that)
* A proper main menu screen as well as game over screens
* A feature to restart the current level in case the player gets stuck (health and current money is saved at the beginning of a level and will be reset as well)
* A custom header file with all important constant values re-used by the game (e.g. enemy speeds, values, ascii numbers, etc.) that can easily be adjusted
