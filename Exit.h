#pragma once
#include "Actor.h"
class Exit : public Actor
{
public:
	Exit(int x, int y, Color col);
	virtual void Draw() override;
	virtual ActorType GetType() override { return ActorType::Exit; }
};

