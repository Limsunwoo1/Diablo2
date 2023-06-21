#pragma once
#include "CSkil.h"

class DiabloSkilBreath : public Skil
{
public:
	DiabloSkilBreath();
	~DiabloSkilBreath();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

private:
	std::vector<class BoltBase*> mSpecialCastSkil;
	std::shared_ptr<class Material>  mMaterial;
	float mInterval;
	float mDeleta;

	float mDeathTime;
};

