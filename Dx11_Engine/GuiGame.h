#pragma once
#include "GuiWidget.h"

namespace gui
{
	class Game : public Widget
	{
	public:
		Game();
		virtual ~Game();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
	};
}

