#pragma once
#include "Point.h"
#include "Actor.h"

class Player : public Actor {

	bool m_keyWarning;
	bool m_hasEasteregg;

	int* m_keyAmount;

	int m_health;
	int m_money;

public:
	Player(int x, int y, Color col);

	int* KeyAmount() { return *&(m_keyAmount); }
	void ResetKeys(); // set amounts to 0
	void PickupKey(Color color, bool isUnid); // key +1 (color based)
	void UseKey(Color color, bool isUnid); // key -1 (color based)

	virtual void Draw() override;
	virtual ActorType GetType() override { return ActorType::Player; }

	bool KeyWarning() { return m_keyWarning; }
	void KeyWarningSwitch();

	bool HasEasteregg() { return m_hasEasteregg; }
	void PickupEasteregg();

	void AddMoney(int money) { m_money += money; }
	void SetMoney(int money) { m_money = money; }
	int GetMoney() { return m_money; }

	void ChangeHealth(int damage) { m_health -= damage; }
	void SetHealth(int health) { m_health = health; }
	int GetHealth() { return m_health; }
	
	void ResetGame() { m_health = 100; m_hasEasteregg = false; m_money = 0; }
};