#pragma once
#include "CGameObject.h"

class GateObject : public GameObject
{
public:
	GateObject();
	virtual ~GateObject();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();
	
	void SetNextScenetype(eSceneType type) { mNextScene = type; }

	void NextScene();
private:
	std::shared_ptr<class Material> mMaterial;
	eSceneType mNextScene;
};

