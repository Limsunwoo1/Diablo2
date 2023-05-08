#include "GuiMeshRenderer.h"
#include "Engin_Source/CMeshRenderer.h"
#include "Engin_Source/CSpriteRenderer.h"

namespace gui
{
	guiMeshRenderer::guiMeshRenderer()
		: gui::Component(eComponentType::MeshRenderer)
	{
		SetName("Renderer");
		SetSize(ImVec2(200.f, 200.f));
	}

	guiMeshRenderer::~guiMeshRenderer()
	{

	}

	void guiMeshRenderer::FixedUpdate()
	{
		gui::Component::FixedUpdate();

		if (GetTarget())
		{
			MeshRenderer* meshRenderer = GetTarget()->GetComponent<MeshRenderer>();
			SpriteRenderer* spriteRenderer = GetTarget()->GetComponent<SpriteRenderer>();

			BaseRenderer* renderer
				= meshRenderer == nullptr ? dynamic_cast<BaseRenderer*>(spriteRenderer) : dynamic_cast<BaseRenderer*>(meshRenderer);

			if (renderer == nullptr)
				return;

			mMaterial = renderer->GetMaterial();
			mMesh = renderer->GetMesh();
		}
	}

	void guiMeshRenderer::Update()
	{
		gui::Component::Update();

		// wstring Çü Name
		std::wstring wMeshName = mMesh->GetName();
		std::wstring wMaterialName = mMaterial->GetName();

		std::string meshName 
			= std::string(wMeshName.begin(), wMeshName.end());
		std::string materialName 
			= std::string(wMaterialName.begin(), wMaterialName.end());

		ImGui::Text("Mesh"); ImGui::SameLine();
		ImGui::InputText("##MeshName", (char*)meshName.data(), meshName.length(), ImGuiInputTextFlags_ReadOnly);

		ImGui::Text("Material"); ImGui::SameLine();
		ImGui::InputText("##Material", (char*)materialName.data(), materialName.length(), ImGuiInputTextFlags_ReadOnly);
	}

	void guiMeshRenderer::LateUpdate()
	{
		gui::Component::LateUpdate();
	}


}