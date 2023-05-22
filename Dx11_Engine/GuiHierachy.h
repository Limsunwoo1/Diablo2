#pragma once
#include "GuiWidget.h"
#include "GuiTreeWidget.h"

#include "..//Engin_Source/CGameObject.h"

namespace gui
{
	class Hierachy : public Widget
	{
	public:
		Hierachy();
		virtual ~Hierachy();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void InitalizeInsepector(void* data);
		void InitalizeScene();
		void AddGameObject(TreeWidget::Node* parent, GameObject* gameObject);

		void UiChilds(GameObject* obj, TreeWidget::Node* root);
	private:
		TreeWidget* mTreeWidget;
	};
}

