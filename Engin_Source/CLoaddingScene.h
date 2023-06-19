#pragma once
#include "CScene.h"

class LoaddingScene : public Scene
{
public:
	LoaddingScene();
	virtual ~LoaddingScene();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void OnEnter() override;

	void SeteSceneType(eSceneType type) { NextScene = type; }
private:
	eSceneType NextScene;
	GameObject* LoadObject;
	float mTime;
};

