#pragma once
#include "..//Engin_Source/CGameObject.h"

class Widget : public GameObject
{
public:
	Widget();
	virtual ~Widget();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

private:

};

