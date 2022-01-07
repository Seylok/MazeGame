#pragma once
#include "Actor.h"

class Door : public Actor
{
	bool m_isOpen;

public:
	Door(int x, int y, Color col);
	virtual void Draw() override;

	virtual ActorType GetType() override { return ActorType::Door; }
	bool IsOpen() { return m_isOpen; }
	void Open() { m_isOpen = true; }
};

