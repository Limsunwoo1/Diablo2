#pragma once
#include "CBoltBase.h"

class LightBolt : public BoltBase
{
public:
	LightBolt();
	~LightBolt();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

	void SetCurObject(GameObject* object) { mCurObject = object; }
	GameObject* GetCurObject() { return mCurObject; }

	void SearchArriveTarget();
	void LightingOn(GameObject* object);
	void LinghtingRun();
	void LightningEnd();
private:
	std::set<GameObject*> mArriveList;
	GameObject* mCurObject;
	GameObject* mNextObject;

	float mDriectionTime;
	float mRadius;

	int mBounceCount;
	int mBounceMaxCount;

	bool mDirection;
	float mYScale;
};