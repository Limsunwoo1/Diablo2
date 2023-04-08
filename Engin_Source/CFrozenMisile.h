#pragma once
#include "CSkil.h"

using namespace Math;
using namespace std;

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

	void SetDirection(Vector2& direction) { mDirection = direction; }
private:
	FrozenOrb* mOrb;

	Math::Vector2 mDirection;
	bool mbMode;
};

