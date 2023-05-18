#include "GuiInspector.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Engin_Source/CRenderer.h"

#include "GuiTransform.h"
#include "GuiMeshRenderer.h"
#include "GuiTexture2D.h"

namespace gui
{
	Inspector::Inspector()
	{
		SetName("Inspector");
		SetSize(ImVec2(300.f, 100.f));

		mComponents.resize((UINT)eComponentType::End);
		mTargetGameObject = Renderer::InspectorGameObject;

		mComponents[(UINT)eComponentType::Transform] = new guiTransform();
		mComponents[(UINT)eComponentType::Transform]->SetTarget(mTargetGameObject);
		AddWidget(mComponents[(UINT)eComponentType::Transform]);

		mComponents[(UINT)eComponentType::MeshRenderer] = new guiMeshRenderer();
		mComponents[(UINT)eComponentType::MeshRenderer]->SetTarget(mTargetGameObject);
		AddWidget(mComponents[(UINT)eComponentType::MeshRenderer]);

		mResources.resize((UINT)eResourceType::End);
		mResources[(UINT)eResourceType::Texture] = new gui::guiTexture2D();
		mResources[(UINT)eResourceType::Texture]->SetName("InspectorTexture");
		AddWidget(mResources[(UINT)eResourceType::Texture]);
	}
	Inspector::~Inspector()
	{
		for (gui::Component* com : mComponents)
		{
			if (com == nullptr)
				continue;

			delete com;
			com = nullptr;
		}

		for (gui::guiResource* resource : mResources)
		{
			if (resource == nullptr)
				continue;

			delete resource;
			resource = nullptr;
		}
	}
	void Inspector::FixedUpdate()
	{

	}
	void Inspector::Update()
	{
	}
	void Inspector::LateUpdate()
	{
	}
	void Inspector::InitalizeTarget(GameObject* target)
	{
	}
	void Inspector::ClearTarget()
	{
		for (gui::Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->SetState(eState::Paused);
			comp->SetTarget(nullptr);
		}

		for (gui::guiResource* res : mResources)
		{
			if (res == nullptr)
				continue;

			res->SetState(eState::Paused);
			res->SetTarget(nullptr);
		}
	}
	void Inspector::InitalizeTargetGameObject()
	{
		ClearTarget();

		mComponents[(UINT)eComponentType::Transform]->SetState(eState::Active);
		mComponents[(UINT)eComponentType::Transform]->SetTarget(mTargetGameObject);

		mComponents[(UINT)eComponentType::MeshRenderer]->SetState(eState::Active);
		mComponents[(UINT)eComponentType::MeshRenderer]->SetTarget(mTargetGameObject);
	}
	void Inspector::InitalizeTargetResource()
	{
		ClearTarget();

		mResources[(UINT)eResourceType::Texture]->SetState(eState::Active);
		mResources[(UINT)eResourceType::Texture]->SetTarget(mTargetResource);
	}
}