#pragma once
#include "CSkil.h"

using namespace Math;

class Flames : public Skil
{
public:
	Flames();
	virtual ~Flames();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;
	virtual void HitSkil(GameObject* obj) override;
private:

};