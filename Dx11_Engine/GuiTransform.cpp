#include "GuiTransform.h"
#include "Engin_Source/CTransform.h"

namespace gui
{
	guiTransform::guiTransform()
		: Component(eComponentType::Transform)
	{
		SetName("Transform");
		SetSize(ImVec2(200.0f, 200.0f));
	}

	guiTransform::~guiTransform()
	{

	}

	void guiTransform::FixedUpdate()
	{
		if (GetTarget() == nullptr)
			return;

		Component::FixedUpdate();

		Transform* tr = GetTarget()->GetComponent<Transform>();

		mPosition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();


	}

	void guiTransform::Update()
	{
		if (GetTarget() == nullptr)
			return;

		Component::Update();

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		if (GetTarget())
		{
			Transform* tr = GetTarget()->GetComponent<Transform>();

			tr->SetPosition(mPosition);
			tr->SetRotation(mRotation);
			tr->SetScale(mScale);
		}
	}

	void guiTransform::LateUpdate()
	{
		if (GetTarget() == nullptr)
			return;

		Component::LateUpdate();
	}

}