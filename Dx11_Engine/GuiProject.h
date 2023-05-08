#pragma once
#include "GuiWidget.h"

namespace gui
{
	class Project : public Widget
	{
	public:
		Project();
		virtual ~Project();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;


	private:
	};
}

