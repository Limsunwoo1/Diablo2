#include "GuiInspector.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "GuiEditor.h"

#include "Engin_Source/CRenderer.h"

#include "GuiTransform.h"
#include "GuiMeshRenderer.h"
#include "GuiTexture2D.h"
#include "..//Engin_Source/CInput.h"

#include <iostream>

extern gui::Editor _Editor;

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
		if (mResources[(UINT)eResourceType::Texture]->GetTarget() == nullptr)
			return;

		ImVec2 MousePos = ImGui::GetMousePos();

		ImVec2 pos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImVec2 size = ImGui::GetContentRegionAvail();

		float diff = windowSize.y - size.y;
		pos.y += diff;

		mResources[(UINT)eResourceType::Texture]->SetSize(ImVec2(size.x, size.y));

		UINT x = 0;
		UINT y = 0;

		ImVec2 curPos = MousePos - pos;
		if (curPos.x < 0 || curPos.y < 0)
			return;

		float fX = curPos.x / size.x;
		float fY = curPos.y / size.y;

		/*u *= dynamic_cast<guiTexture2D*>(mResources[(UINT)eResourceType::Texture])->GetMaxIndexX();
		v *= dynamic_cast<guiTexture2D*>(mResources[(UINT)eResourceType::Texture])->GetMaxIndexY();*/

		fX *= 5;
		fY *= 37;

		x = fX;
		y = fY;


		if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
		{
			Texture2D* tex = dynamic_cast<Texture2D*>(mResources[(UINT)eResourceType::Texture]->GetTarget());
			_Editor.SetTileMaxX(tex->GetMaxX());
			_Editor.SetTileMaxY(tex->GetMaxY());

			_Editor.SetTileIndexX(x);
			_Editor.SetTileIndexY(y);

			std::cout << x << "   X Index" << y << "    Y Idex  " << std::endl;
		}
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