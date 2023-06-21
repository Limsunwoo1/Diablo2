#pragma once
#include "CButton.h"

class SkiltreeButton : public Button
{
public:
	SkiltreeButton();
	virtual ~SkiltreeButton();

	void InitTexture(std::weak_ptr<Texture2D> tex);
	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void SetSkilType(eSkilList type) { mSkilType = type; }
	eSkilList GetSkilType() { return mSkilType; }

	void SetPoint(int point) { mPoint = point; }
	void SetMaxPoint(int max) { mMaxPoint = max; }

	UINT GetPoint() { return mPoint; }
	UINT GetMaxPoint() {return  mMaxPoint; }

	void UpPoint() { mPoint += 1; }

	const std::weak_ptr<Texture2D>& GetTex();

private:

	UINT mPoint;
	UINT mMaxPoint;

	eSkilList mSkilType;

	std::shared_ptr<class Material> mMaterial;
};

