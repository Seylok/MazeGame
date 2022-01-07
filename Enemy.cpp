#include <iostream>
#include "Enemy.h"
#include <Windows.h>

Enemy::Enemy(int x, int y, Color col, int deltaX, int deltaY, int rDir, int rDelay, int rSpeed)
	: Actor(x, y, col)
	, m_currentMovementX(0)
	, m_currentMovementY(0)
	, m_directionX(0)
	, m_directionY(0)
	, m_movementInX(deltaX)
	, m_movementInY(deltaY)
	, m_startingDir(rDir)
	, m_randomDelay(rDelay)
	, m_speed(rSpeed)
	, m_counter(0)
{
	// randomize starting direction
	if (m_movementInX != 0) {
		if (m_startingDir == 0) {
			m_directionX = -1;
		} else m_directionX = 1;
	}
	if (m_movementInY != 0) {
		if (m_startingDir == 0) {
			m_directionY = -1;
		}
		else m_directionY = 1;
	}
}

void Enemy::Draw() {
	hlp.ColoredChar(hlp.EMY, m_color);
}

void Enemy::Update() {
	bool validLocation = true;
	if (m_randomDelay > 0)
	{
		m_randomDelay--; // "turn" delay before starting movement, makes everything a little less predictable and repetitive
	}
	else {
		m_counter++;
		if (m_counter > m_speed) { // movement speed (lower value = faster, since it's the amount of skipped updates)
			m_counter = 0;

			if (m_movementInX != 0) {
				UpdateDirection(m_currentMovementX, m_directionX, m_movementInX);
				// additional check if an enemy would hit an obstacle on its current movement cycle via reading the console output window
				// unfortunately, this doesn't seem to work with any displayed extended ascii char :/
				short posX = (short)(m_pPosition->x + m_directionX + hlp.kOffsetX);
				short posY = (short)(m_pPosition->y + hlp.kOffsetY);
				char chr = hlp.ReadOutput(posX, posY);
				if (chr == ' ' || chr == hlp.kPlayer) {
					// do nothing
				}
				else {
					// ignore movement this update and reverse direction
					m_directionX *= -1;
					m_currentMovementX = 0;
					validLocation = false;
				}
			}
			if (m_movementInY != 0) {
				UpdateDirection(m_currentMovementY, m_directionY, m_movementInY);
				short posX = (short)(m_pPosition->x + hlp.kOffsetX);
				short posY = (short)(m_pPosition->y + m_directionY + hlp.kOffsetY);
				char chr = hlp.ReadOutput(posX, posY);
				if (chr == ' ' || chr == hlp.kPlayer) {
					// do nothing
				}
				else {
					m_directionY *= -1;
					m_currentMovementY = 0;
					validLocation = false;
				}
			}
			if (validLocation) this->SetPosition(m_pPosition->x + m_directionX, m_pPosition->y + m_directionY);
		}
	}
}

void Enemy::UpdateDirection(int& current, int& direction, int& movement) {
	if (std::abs(current) >= movement) {
		// reverse direction
		current = movement * direction;
		direction *= -1;
	}
	current += direction;
}