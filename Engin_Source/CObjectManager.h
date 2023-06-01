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

	void SetOffsetData(eWallType type, Vector2 offset);
	Vector2 GetOffsetData(eWallType type);

	void SetSizeData(eWallType type, Vector2 size);
	Vector2 GetSizeData(eWallType type);

private:
	queue<Skil*> SkilContainer;
	vector<Vector2> ObjectOffsetData;
	vector<Vector2> ObjectSizeData;
};

