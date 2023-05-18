#pragma once
#include "GuiWidget.h"
#include "Engin_Source/CGameObject.h"
#include "GuiComponent.h"
#include "GuiResource.h"

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
		GameObject* GetTargetGameObject() { return mTargetGameObject; }
		void SetTargetGameObject(GameObject* target) { mTargetGameObject = target; }

		Resource* GetTargetResource() { return mTargetResource; }
		void SetTargetResource(Resource* resource) { mTargetResource = resource; }

		void ClearTarget();
		void InitalizeTargetGameObject();
		void InitalizeTargetResource();
		
	private:
		GameObject* mTargetGameObject;
		Resource* mTargetResource;

		std::vector<gui::Component*> mComponents;
		std::vector<gui::guiResource*> mResources;
	};
}

