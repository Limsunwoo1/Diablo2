#pragma once
#include "GuiWidget.h"


namespace gui
{
	class DockEditor : public Widget
	{
	public:
		DockEditor();
		virtual ~DockEditor();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;


	private:
		bool mbPadding;
		bool mbFullScreen;

		ImGuiDockNodeFlags mDockSpace_Flags;
	};
}

