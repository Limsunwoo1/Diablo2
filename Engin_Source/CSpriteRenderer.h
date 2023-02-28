#pragma once
#include "CBaseRenderer.h"


class SpriteRenderer : public BaseRenderer
{
public:
	SpriteRenderer();
	virtual ~SpriteRenderer();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

private:

};

