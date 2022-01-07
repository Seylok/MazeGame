#pragma once
#include "Actor.h"
class Key : public Actor
{
public:
	Key(int x, int y, Color col, bool isUnid);
	virtual void Draw() override;
	virtual ActorType GetType() override { return ActorType::Key; }
	bool isUnidentified;
};