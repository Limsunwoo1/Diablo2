#pragma once
#include "GuiComponent.h"

namespace gui
{
	class Transform : public gui::Component
	{
	public:
		Transform();
		virtual ~Transform();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;


	private:
		Math::Vector3 Position;
		Math::Vector3 Rotation;
		Math::Vector3 Scale;
	};
}

