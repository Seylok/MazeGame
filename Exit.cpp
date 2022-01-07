#include <iostream>
#include "Exit.h"

Exit::Exit(int x, int y, Color col)
	: Actor(x, y, col)
{
}

void Exit::Draw() {
	hlp.ColoredChar(hlp.EXT, m_color);
}