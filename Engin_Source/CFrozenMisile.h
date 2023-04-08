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
	
	void RunMisile();
	const wstring& GetKeyName() { return mKeyName; }
private:
	FrozenOrb* mOrb;

	bool mbMode;
	wstring mKeyName;
};

