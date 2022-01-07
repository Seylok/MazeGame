#include <iostream>
#include "Player.h"

Player::Player(int x, int y, Color col)
	: Actor(x, y, col)
	, m_keyAmount(new int[4])
	, m_keyWarning(false)
	, m_hasEasteregg(false)
	, m_health(hlp.kPlayerStartingHealth)
	, m_money(0)
{
}

// increase amount of keys
void Player::PickupKey(Color color, bool isUnid) {
	if (color == Color::Red && !isUnid) m_keyAmount[0]++;
	if (color == Color::Green && !isUnid) m_keyAmount[1]++;
	if (color == Color::Blue && !isUnid) m_keyAmount[2]++;
	if (color == Color::White && !isUnid) m_keyAmount[3]++;
	if (color == Color::Red && isUnid) m_keyAmount[4]++;
	if (color == Color::Green && isUnid) m_keyAmount[5]++;
	if (color == Color::Blue && isUnid) m_keyAmount[6]++;
	if (color == Color::White && isUnid) m_keyAmount[7]++;
}

// decrease amount of keys
void Player::UseKey(Color color, bool isUnid) {
	if (color == Color::Red && !isUnid) m_keyAmount[0]--;
	if (color == Color::Green && !isUnid) m_keyAmount[1]--;
	if (color == Color::Blue && !isUnid) m_keyAmount[2]--;
	if (color == Color::White && !isUnid) m_keyAmount[3]--;
	if (color == Color::Red && isUnid) m_keyAmount[4]--;
	if (color == Color::Green && isUnid) m_keyAmount[5]--;
	if (color == Color::Blue && isUnid) m_keyAmount[6]--;
	if (color == Color::White && isUnid) m_keyAmount[7]--;
}

// reset amount of keys
void Player::ResetKeys() {
	m_keyAmount[0] = 0;
	m_keyAmount[1] = 0;
	m_keyAmount[2] = 0;
	m_keyAmount[3] = 0;
	m_keyAmount[4] = 0;
	m_keyAmount[5] = 0;
	m_keyAmount[6] = 0;
	m_keyAmount[7] = 0;
}

// draw the player character
void Player::Draw() {
	hlp.ColoredChar(hlp.kPlayer, Color::Player);
}

// set easteregg to picked up state
void Player::PickupEasteregg() {
	m_hasEasteregg = true;
}

// enable/disable info message the player needs a key
void Player::KeyWarningSwitch() {
	m_keyWarning = !m_keyWarning;
}