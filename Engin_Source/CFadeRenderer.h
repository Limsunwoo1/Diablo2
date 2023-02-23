#pragma once
#include "CBaseRenderer.h"

class FadeRenderer : public BaseRenderer
{
public:
	FadeRenderer();
	virtual ~FadeRenderer();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

private:
	bool bfade;
	float mDelta;
};