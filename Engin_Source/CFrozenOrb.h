#pragma once
#include "CSkil.h"

using namespace Math;
using namespace std;

class FrozenMissile;
class FrozenOrb : public Skil
{
public:
	FrozenOrb();
	virtual ~FrozenOrb();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

	void RunOrb();

	void OnOrb();
	void RunningOrb();
	void OffOrb();

	void SetDirection(Vector2& direction) { mDirection = direction; }
private:
	vector<FrozenMissile*> mFrozenMissile;
	vector<FrozenMissile*> mSpeakerMissile;
	Vector2 mDirection;

	float mRunningTime;
	int mFrozenIndex;
	int mSpeakerIndex;
	bool mbOnOrb;
	bool mbOff;
};

