#pragma once
#include "GuiWidget.h"

namespace gui
{
	class Hierarchy : public Widget
	{
	public:
		Hierarchy();
		virtual ~Hierarchy();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:

	};
}
