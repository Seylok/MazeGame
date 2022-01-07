#pragma once
#include "Actor.h"

class Money : public Actor
{
	int m_worth;

public:
	Money(int x, int y, Color col, int worth);

	int GetWorth() const { return m_worth; }

	virtual void Draw() override;
	virtual ActorType GetType() override { return ActorType::Money; }
};

