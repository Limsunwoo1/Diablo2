#pragma once
#include "CSkil.h"

class ShockHit : public Skil
{
public:
	ShockHit();
	~ShockHit();

	virtual void Initalize() override;
	virtual void Update() override;

	virtual void InitAnimation() override;

	void SetTarget(GameObject* obj);
private:
	float mDeathTime;
	std::shared_ptr<class Material> mMaterial;
	class Transform* mTargetTr;
};

