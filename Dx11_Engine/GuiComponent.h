#pragma once
#include "GuiWidget.h"
#include "Engin_Source/CComponent.h"

namespace gui
{
	class Component : public Widget
	{
	public:
		Component(eComponentType type);
		virtual ~Component();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetTarget(GameObject* target) { mTarget = target; }
		GameObject* GetTarget() { return mTarget; }

	private:
		eComponentType mType;
		GameObject* mTarget;
	};
}


