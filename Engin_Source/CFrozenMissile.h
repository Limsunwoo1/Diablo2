#pragma once
#include "CSkil.h"

using namespace Math;
using namespace std;

enum class eMisileMode
{
	Straight,
	Rotation,
	ReversRotation,
};

class FrozenOrb;
class FrozenMissile : public Skil
{
public:
	FrozenMissile();
	virtual ~FrozenMissile();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;
	
	void RunMisile();
	const wstring& GetKeyName() { return mKeyName; }
	void SetMode(eMisileMode mode) { mbMode = mode; }
	void SetTime(float time) { mTime = time; }

	virtual void HitSkil(GameObject* obj) override;

	void SetHit(bool hit) { mHit = hit; }
private:

private:
	FrozenOrb* mOrb;
	wstring mKeyName;

	eMisileMode mbMode;
	float mTime;

	bool mHit;
};

