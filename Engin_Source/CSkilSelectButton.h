#pragma once
#include "CButton.h"


class SkilSelectButton : public Button
{
public:
	SkilSelectButton();
	virtual ~SkilSelectButton();

	void SetSkilType(eSkilList type) { mType = type; }
	eSkilList GetSkilType() { return mType; }
	void InItTex(std::weak_ptr<Texture2D> tex);
	
	 const std::weak_ptr<Material>& GetMaterial() const { return mMaterial; }
	 void To_SkilSelectPanel();
private:
	std::shared_ptr<Material> mMaterial;
	eSkilList mType;
};

