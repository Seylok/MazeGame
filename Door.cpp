#include <iostream>
#include "Door.h"

Door::Door(int x, int y, Color col)
	: Actor(x, y, col)
	, m_isOpen(false)
{
}

void Door::Draw() {
	if (m_isOpen) hlp.ColoredChar(hlp.DOO, Color::OpenDoor);
	else hlp.ColoredChar(hlp.DOR, m_color);
}