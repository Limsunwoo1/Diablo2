#pragma once
#include "CSkil.h"

using namespace Math;
using namespace std;

class FrozenMisile;
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
	vector<FrozenMisile*> mFrozenMisile;
	vector<FrozenMisile*> mSpeakerMisile;
	Vector2 mDirection;

	float mRunningTime;
	int mSpeakerIndex;
	bool mbOnOrb;
	bool mbOff;
};

