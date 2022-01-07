#pragma once
#include "Actor.h"
class Enemy : public Actor
{
	int m_movementInX;
	int m_movementInY;
	int m_currentMovementX;
	int m_currentMovementY;

	int m_directionX;
	int m_directionY;
	int m_startingDir;

	int m_randomDelay;
	int m_counter;
	int m_speed;

	void UpdateDirection(int& current, int& direction, int& movement);

public:
	Enemy(int x, int y, Color col, int deltaX = 0, int deltaY = 0, int rDir = 0, int rDelay = 0, int rSpeed = 0);

	virtual ActorType GetType() override { return ActorType::Enemy; }
	virtual void Draw() override;
	virtual void Update() override;
};