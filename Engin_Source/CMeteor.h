#pragma once
#include "CSkil.h"
#include "CGenericAnimator.h"

using namespace Math;
using namespace std;


class Flames;
class FireTargetPin;
class Meteor : public Skil
{
public:
	Meteor();
	virtual ~Meteor();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

	void RunMeteor();

	void SetPinPos(Math::Vector3& pos);// { mPinPos = pos; }
	void SetPinPos(float& x, float& y, float& z); //{ mPinPos = Vector3(x,y,z); }
	void SetSpeed();

	void OnMeteor();
	void OffMeteor();

	FireTargetPin* GetMetorTargetPin() { return mTargetPin; }

	virtual void HitSkil(GameObject* obj) override;
private:
	FireTargetPin* mTargetPin;
	std::vector<Flames*> mFlames;

	Math::Vector3 mPinPos;
	float mSpeed;
	bool mbRun;
};

