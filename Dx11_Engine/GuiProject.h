#pragma once
#include "GuiWidget.h"
#include "GuiTreeWidget.h"

#include "..//Engin_Source/CResourceManager.h"
#include "..//Engin_Source/CResource.h"

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

		void ResetContent();
	private:
		template <typename T>
		void AddResources(GuiTreeWidget::Node* rootNode, const char* name)
		{
			std::vector<std::shared_ptr<T>> resources = ResourceManager::GetInstance()->Finds<T>();
			GuiTreeWidget::Node* stemNode = mTreeWidget->AddNode(rootNode, name, 0, true);

			for (std::shared_ptr<T> resource : resources)
			{
				std::string name = std::string(resource->Entity::GetName().begin(), resource->Entity::GetName().end());
				mTreeWidget->AddNode(stemNode, name, (void*)resource.get());
			}
		}

		void toInspector();

	private:
		GuiTreeWidget* mTreeWidget;
	};
}

