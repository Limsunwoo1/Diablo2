#include "GuiMeshRenderer.h"
#include "Engin_Source/CMeshRenderer.h"
#include "Engin_Source/CSpriteRenderer.h"

#include "GuiEditor.h"
#include "GuiListWidget.h"
#include "GuiInspector.h"

#include "..//Engin_Source/CResourceManager.h"
#include "..//Engin_Source/CResource.h"

extern gui::Editor _Editor;

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
		if (GetTarget() == nullptr)
			return;

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
		if (GetTarget() == nullptr)
			return;

		if (mMesh.lock() == nullptr || mMaterial.lock() == nullptr)
			return;

		gui::Component::Update();

		// wstring 형 Name
		std::wstring wMeshName = mMesh.lock()->GetName();
		std::wstring wMaterialName = mMaterial.lock()->GetName();

		std::string meshName 
			= std::string(wMeshName.begin(), wMeshName.end());
		std::string materialName 
			= std::string(wMaterialName.begin(), wMaterialName.end());

		ImGui::Text("Mesh");
		ImGui::InputText("##MeshName", (char*)meshName.data(), meshName.length(), ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();

		if (ImGui::Button("##MeshBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* listUi = _Editor.GetWidget<ListWidget>("ListWidget");
			listUi->SetState(eState::Active);

			// 모든 메쉬의 리소스를 가져와야한다
			std::vector<std::shared_ptr<Mesh>> meshes = ResourceManager::GetInstance()->Finds<Mesh>();

			std::vector<std::wstring> wName;
			for (auto mesh : meshes)
			{
				wName.push_back(mesh->GetName());
			}

			listUi->SetItemList(wName);
			listUi->SetEvent(this, std::bind(&gui::guiMeshRenderer::SetMesh, this, std::placeholders::_1));
		}

		ImGui::Text("Material");
		ImGui::InputText("##Material", (char*)materialName.data(), materialName.length() + 20, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();

		if (ImGui::Button("##MaterialBtn", ImVec2(15.0f, 15.0f)))
		{
			ListWidget* listUi = _Editor.GetWidget<ListWidget>("ListWidget");
			listUi->SetState(eState::Active);

			// 모든 메쉬의 리소스를 가져와야한다
			std::vector<std::shared_ptr<Material>> meshes = ResourceManager::GetInstance()->Finds<Material>();

			std::vector<std::wstring> wName;
			for (auto mesh : meshes)
			{
				wName.push_back(mesh->GetName());
			}

			listUi->SetItemList(wName);
			listUi->SetEvent(this, std::bind(&gui::guiMeshRenderer::SetMaterial, this, std::placeholders::_1));
		}
	}

	void guiMeshRenderer::LateUpdate()
	{
		if (GetTarget() == nullptr)
			return;

		gui::Component::LateUpdate();
	}
	void guiMeshRenderer::SetMesh(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(wKey);

		Inspector* insptector = _Editor.GetWidget<Inspector>("Inspector");
		insptector->GetTargetGameObject()->GetComponent<MeshRenderer>()->SetMesh(mesh);
	}
	void guiMeshRenderer::SetMaterial(std::string key)
	{
		std::wstring wKey(key.begin(), key.end());
		std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(wKey);

		Inspector* insptector = _Editor.GetWidget<Inspector>("Inspector");
		insptector->GetTargetGameObject()->GetComponent<MeshRenderer>()->SetMaterial(material);
	}
}