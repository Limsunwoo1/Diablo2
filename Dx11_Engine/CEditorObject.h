#pragma once
#include "..//Engin_Source/CGameObject.h"


class EditorObject : public GameObject
{
public:
	EditorObject();
	virtual ~EditorObject();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

private:

};

