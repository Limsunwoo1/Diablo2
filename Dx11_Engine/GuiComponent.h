#pragma once
#include "GuiWidget.h"

class Component;
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


