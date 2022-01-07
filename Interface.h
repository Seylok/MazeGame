#pragma once

class Interface {
public:
	Interface();
	~Interface();

	int WelcomeScreen(int finalLevel);
	void GameOverScreen(bool easteregg, int money, int health);
	void DeathScreen();
	void Inventory(int currentLevel, int pPosX, int pPosY, int* keys, bool easteregg, std::string levelName, int health, int money);
	void Legend(bool keyWarning);
};