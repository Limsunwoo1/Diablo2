#pragma once
#include "CSkil.h"

class AndarielSkil : public Skil
{
private:
	AndarielSkil();
	~AndarielSkil();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

public:
	std::vector<class BoltBase*> mSpecialCastSkil;
	float mInterval;
	float mDeleta;
};

