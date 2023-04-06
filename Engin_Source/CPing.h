#pragma once
#include "CGameObject.h"


class Ping : public GameObject
{
public:
	Ping();
	virtual ~Ping();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	virtual void InitAnimation();

	void SetPos(const Math::Vector2 pos) { mPos = pos; }
private:
	float mSize;
	float mTime;

	Math::Vector2 mPos;
};

