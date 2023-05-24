#pragma once
#include "GuiTexture2D.h"

namespace gui
{
	class ObejctList : public guiTexture2D
	{
	public:
		ObejctList();
		virtual ~ObejctList();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
	private:
		guiTexture2D* mObjectTex;
	};
}

