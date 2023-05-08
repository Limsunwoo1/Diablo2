#include "GuiMeshRenderer.h"
#include "Engin_Source/CMeshRenderer.h"

namespace gui
{
	guiMeshRenderer::guiMeshRenderer()
		: gui::Component(eComponentType::MeshRenderer)
	{
		SetName("MeshRenderer");
		SetSize(ImVec2(200.f, 120.f));
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

			mMaterial = meshRenderer->GetMaterial();
			mMesh = meshRenderer->GetMesh();
		}
	}

	void guiMeshRenderer::Update()
	{
		gui::Component::Update();

		std::string meshName = std::string(mMesh.lock()->GetName().begin(), mMesh.lock()->GetName().end());
		std::string materialName = std::string(mMaterial.lock()->GetName().begin(), mMaterial.lock()->GetName().end());

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