#pragma once
#include "CSkil.h"

class FireTargetPin : public Skil
{
public:
	FireTargetPin(Player* player);
	virtual ~FireTargetPin();

	virtual void Initalize() override;
	virtual void Update() override; 
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

private:
};

