#pragma once
#include "CSkil.h"

class DiabloSkilFireStom : public Skil
{
public:
	DiabloSkilFireStom();
	~DiabloSkilFireStom();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

private:
	std::vector<class BoltBase*> mSpecialCastSkil;
	std::shared_ptr<class Material>  mMaterial;
	float mDeleta;

	float mDeathTime;
};

