#pragma once
#include "CSkil.h"
#include "CGenericAnimator.h"

class Flames;
class FireTargetPin;
class Meteor : public Skil
{
public:
	Meteor();
	virtual ~Meteor();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

private:
	FireTargetPin* mTargetPin;
	std::vector<Flames> mFlames;
};

