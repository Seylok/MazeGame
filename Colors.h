#pragma once

// colors - ENUMS
enum struct Color {
	Black = 0,
	DarkMagenta = 5, EnemyW = DarkMagenta, // weak enemy
	DarkYellow = 6, Money = DarkYellow, Name = DarkYellow, // money & names
	Default = 7, // e.g. texts
	Grey = 8, Wall = Grey, OpenDoor = Grey, // walls
	Blue = 9, // key & door
	Green = 10, // key & door
	Turquoise = 11, Player = Turquoise, // player, unidentified keys
	Red = 12, Warning = Red, // warnings, key & Door
	Magenta = 13, EnemyS = Magenta, // strong enemy
	Yellow = 14, Exit = Yellow, // exit
	White = 15 // spawners, key & Door
};