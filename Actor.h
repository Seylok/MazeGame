#pragma once
#include "Point.h"
#include "Helpers.h"

enum class ActorType {
	Enemy,
	Player,
	Door,
	Exit,
	Key,
	Money
};

class Actor {
protected:
	Point* m_pPosition;
	Color m_color;
	bool m_IsActive;

public:
	Actor(int x, int y, Color color);
	virtual ~Actor();

	int GetPositionX();
	int GetPositionY();
	void SetPosition(int x, int y);
	Color GetColor() { return m_color; }

	void Remove() { m_IsActive = false;  }
	bool IsActive() { return m_IsActive;  }

	virtual ActorType GetType() = 0;
	virtual void Draw() = 0;
	virtual void Update() { }
};