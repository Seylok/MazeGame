#include <iostream>
#include "Key.h"

Key::Key(int x, int y, Color col, bool isUnid)
	: Actor(x, y, col)
	, isUnidentified(isUnid)
{
}

void Key::Draw() {
	if (isUnidentified) {
		hlp.ColoredChar(hlp.KEY, Color::Turquoise);
	}
	else hlp.ColoredChar(hlp.KEY, m_color);
}