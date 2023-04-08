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

	void SetDirection(Vector2& direction) { mDirection = direction; }
private:
	vector<FrozenMisile*> mFrozenMisile;
	Vector2 mDirection;
	vector<Vector2> mMisileDirection;
};

