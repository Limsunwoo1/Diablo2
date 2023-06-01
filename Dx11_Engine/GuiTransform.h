#pragma once
#include "GuiComponent.h"

namespace gui
{
	class guiTransform : public gui::Component
	{
	public:
		guiTransform();
		virtual ~guiTransform();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;


	private:
		Math::Vector3 mPosition;
		Math::Vector3 mRotation;
		Math::Vector3 mScale;
		Math::Vector3 mSize;
		Math::Vector3 mOffset;
	};
}

