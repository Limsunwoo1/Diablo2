#pragma once
#include "GuiWidget.h"
#include "GuiTreeWidget.h"

#include "..//Engin_Source/CResourceManager.h"
#include "..//Engin_Source/CResource.h"

#include "..//Engin_Source/CEntity.h"

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
		void AddResources(TreeWidget::Node* rootNode, const char* name)
		{
			const std::vector<std::shared_ptr<T>> resources = ResourceManager::GetInstance()->Finds<T>();
			TreeWidget::Node* stemNode = mTreeWidget->AddNode(rootNode, name, 0, true);

			for (std::shared_ptr<T> resource : resources)
			{
				std::wstring wName = resource->GetName();
				std::string sName(wName.begin(), wName.end());
				mTreeWidget->AddNode(stemNode, sName, resource.get());
			}
		}

		template <typename T>
		void AddTileResources(TreeWidget::Node* rootNode, const char* name)
		{
			const std::vector<std::shared_ptr<T>> resources = ResourceManager::GetInstance()->FindsTileTex<T>();
			TreeWidget::Node* stemNode = mTreeWidget->AddNode(rootNode, name, 0, true);

			for (std::weak_ptr<T> resource : resources)
			{
				std::wstring wName = resource.lock()->GetName();
				std::string sName(wName.begin(), wName.end());
				mTreeWidget->AddNode(stemNode, sName, resource.lock().get());
			}
		}

		void toInspector(void* data);

	private:
		TreeWidget* mTreeWidget;
	};
}

