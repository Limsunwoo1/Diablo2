#pragma once
#include "CEntity.h"

class GameObject;
class Component
{
public:
	enum class eType
	{
		None,
		Transform,
		Camera,

		//reder
		Mesh,
		SkyBox,
		Decal,

		Script,
		End,
	};

	Component();
	virtual ~Component();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

private:
	const eType mType;
	GameObject* mOwner;
};

