#pragma once
#include "CEngine.h"
#include "CGameObject.h"
#include "CSkil.h"


using namespace std;

class ObjectManager
{
	SINGLE(ObjectManager);
public:

	void Update();
	void FixedUpdate();
	void Initialize();

	void AddSKilObject(GameObject* obj)
	{
		Skil* skil = dynamic_cast<Skil*>(obj);
		if (!skil)
			return;

		skil->Death();
		SkilContainer.push(skil);
	}
	Skil* GetSkilObj(Player* owner);



private:
	queue<Skil*> SkilContainer;
};

