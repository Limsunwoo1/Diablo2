#pragma once
#include "GuiWidget.h"

namespace gui
{
	class Consol : public Widget
	{
	public:
		Consol();
		virtual ~Consol();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:

	};
}

