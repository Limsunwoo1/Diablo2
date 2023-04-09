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
class FrozenMisile : public Skil
{
public:
	FrozenMisile();
	virtual ~FrozenMisile();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;
	
	void RunMisile();
	const wstring& GetKeyName() { return mKeyName; }
	void SetMode(eMisileMode mode) { mbMode = mode; }
	void SetTime(float time) { mTime = time; }
private:
	FrozenOrb* mOrb;
	wstring mKeyName;

	eMisileMode mbMode;
	float mTime;
};

