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

		UINT GetMaxIndexX() { return mMaxIndexX; }
		UINT GetMaxIndexY() { return mMaxIndexY; }
		void SetMiaxIndeX(UINT x) { mMaxIndexX = x; }
		void SetMaxIndexY(UINT y) { mMaxIndexY = y; }
		

	private:
		UINT mMaxIndexX;
		UINT mMaxIndexY;
	};
}

