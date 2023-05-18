#pragma once
#include "GuiResource.h"

namespace gui
{
	class guiTexture2D : public guiResource
	{
	public:
		guiTexture2D();
		virtual ~guiTexture2D();

		virtual void Update() override;

	private:

	};
}

