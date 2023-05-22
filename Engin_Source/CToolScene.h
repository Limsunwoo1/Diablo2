#pragma once
#include "CScene.h"

class ToolScene : public Scene
{
public:
	ToolScene();
	virtual ~ToolScene();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void OnEnter() override;
	//virtual void OnExit() {};


private:
	class TilePallet* mTilePallet;
};
