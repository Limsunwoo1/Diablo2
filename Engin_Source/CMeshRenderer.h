#pragma once
#include "CBaseRenderer.h"

class MeshRenderer : public BaseRenderer
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

private:
};