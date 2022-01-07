#pragma once
#include "Level.h"
#include "Player.h"
#include "Interface.h"
#include "Input.h"

class Game {
	bool m_gameOver;
	bool m_hasDied;

public:
	Player m_player;
	Level m_level;
	Interface m_interface;

	Game();
	~Game();

	void Run();

private:
	bool Update();
	void Draw();
	bool CollisionHandler(int newPlayerX, int newPlayerY);

	int m_healthAtStart;
	int m_moneyAtStart;
};