#pragma once
#include "CScript.h"

class Camera;
class GridScript : public Script
{
public:
	GridScript();
	virtual ~GridScript();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void SetCamera(Camera* camera) { mCamera = camera; }


private:
	Camera* mCamera;
};