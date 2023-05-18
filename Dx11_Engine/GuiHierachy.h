#pragma once
#include "GuiWidget.h"
#include "GuiTreeWidget.h"

#include "..//Engin_Source/CGameObject.h"

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

		void InitalizeInsepector(void* data);
		void InitalizeScene();
		void AddGameObject(TreeWidget::Node* parent, GameObject* gameObject);

	private:
		TreeWidget* mTreeWidget;
	};
}

