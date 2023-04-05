#pragma once
#include "CEngine.h"
#include "CGameObject.h"
#include "CSkilFire.h"


using namespace std;

class ObjectManager
{
	SINGLE(ObjectManager);
public:

	void Update();
	void FixedUpdate();
	void Initialize();

	template <typename T>
	void AddSKilObject(GameObject* obj)
	{
		SkilFire* skil = dynamic_cast<T*>(obj);
		if (!skil)
			return;

		skil->Death();
		SkilContainer.push(skil);
	}
	SkilFire* GetSkilObj(Player* owner);



private:
	queue<SkilFire*> SkilContainer;
};

