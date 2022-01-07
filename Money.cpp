#include <iostream>
#include "Money.h"

Money::Money(int x, int y, Color col, int worth)
	: Actor(x, y, col)
	, m_worth(worth)
{
}

void Money::Draw() {
	hlp.ColoredChar(hlp.MON, m_color);
}