#include <iostream>
#include "Actor.h"

Actor::Actor(int x, int y, Color color)
	: m_pPosition(new Point(x, y))
	, m_color(color)
	, m_IsActive(true)
{
}
Actor::~Actor()
{
	delete m_pPosition;
	m_pPosition = nullptr;
}

int Actor::GetPositionX() {
	return m_pPosition->x;
}

int Actor::GetPositionY() {
	return m_pPosition->y;
}

void Actor::SetPosition(int x, int y) {
	// only update position if it has changed since the last check (prevents flickering from continued clearing & re-drawing)
	if (m_pPosition->x != x || m_pPosition->y != y) {
		hlp.ClearCursorPosition(m_pPosition->x + hlp.kOffsetX, m_pPosition->y + hlp.kOffsetY); // clear tile on old actor position
		m_pPosition->x = x;
		m_pPosition->y = y;
	}
}