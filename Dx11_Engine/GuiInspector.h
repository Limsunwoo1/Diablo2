#pragma once
#include "GuiWidget.h"
#include "Engin_Source/CGameObject.h"
#include "GuiComponent.h"

namespace gui
{
	class Inspector : public Widget
	{
	public:
		Inspector();
		virtual ~Inspector();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void InitalizeTarget(GameObject* target);
		GameObject* GetTarget() { return mTarget; }

	private:
		GameObject* mTarget;
		std::vector<gui::Component*> mComponents;
	};
}

