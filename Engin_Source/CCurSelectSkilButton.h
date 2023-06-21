#pragma once
#include "CButton.h"
class CurSelectSkilButton : public Button
{
public:
	CurSelectSkilButton();
	virtual ~CurSelectSkilButton();

	void SetSkilType(eSkilList type) { mType = type; }
	eSkilList GetSkilType() { return mType; }
	void InItTex(std::weak_ptr<Texture2D> tex);
	virtual void Update() override;

	const std::weak_ptr<Material>& GetMaterial() const { return mMaterial; }
	void OpenSelectPanel();
private:
	std::shared_ptr<Material> mMaterial;
	eSkilList mType;
};

