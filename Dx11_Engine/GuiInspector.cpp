#include "GuiInspector.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Engin_Source/CRenderer.h"

#include "GuiTransform.h"
#include "GuiMeshRenderer.h"

namespace gui
{
	Inspector::Inspector()
	{
		SetName("Inspector");

		mComponents.resize((UINT)eComponentType::End);
		mTarget = Renderer::InspectorGameObject;

		mComponents[(UINT)eComponentType::Transform] = new guiTransform();
		mComponents[(UINT)eComponentType::Transform]->SetTarget(mTarget);
		AddWidget(mComponents[(UINT)eComponentType::Transform]);

		mComponents[(UINT)eComponentType::MeshRenderer] = new guiTransform();
		mComponents[(UINT)eComponentType::MeshRenderer]->SetTarget(mTarget);
		AddWidget(mComponents[(UINT)eComponentType::MeshRenderer]);


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
}