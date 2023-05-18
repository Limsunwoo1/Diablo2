#pragma once
#include "GuiWidget.h"
#include "..//Engin_Source/CResource.h"


namespace gui
{
	class guiResource : public Widget
	{
	public:
		guiResource();
		virtual ~guiResource();

		virtual void FixedUpdate() override;

		void SetTarget(Resource* target) { mTarget = target; }
		Resource* GetTarget() { return mTarget; }

	private:
		Resource* mTarget;

	};
}

