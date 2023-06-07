#include "GuiTransform.h"
#include "Engin_Source/CTransform.h"
#include "..//Engin_Source/CWallObject.h"

#include "..//Engin_Source/CInput.h"

using namespace Math;
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


		Transform* tr = GetTarget()->GetComponent<Transform>();

		if (tr == nullptr)
			return;

		Component::FixedUpdate();
		mPosition = tr->GetPosition();
		mRotation = tr->GetRotation();
		mScale = tr->GetScale();
		mSize = tr->GetSize();
	}

	void guiTransform::Update()
	{
		if (GetTarget() == nullptr)
			return;

		Transform* tr = GetTarget()->GetComponent<Transform>();
		if (tr == nullptr)
			return;

		Component::Update();

		ImGui::Text("Position"); ImGui::SameLine();
		ImGui::InputFloat3("##Position", (float*)&mPosition);

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::InputFloat3("##Rotation", (float*)&mRotation);

		ImGui::Text("Scale"); ImGui::SameLine();
		ImGui::InputFloat3("##Scale", (float*)&mScale);

		ImGui::Text("Size"); ImGui::SameLine();
		ImGui::InputFloat3("##Size", (float*)&mSize);

		WallObject* wall = dynamic_cast<WallObject*>(GetTarget());
		if (wall != nullptr)
		{
			Vector2 offset = wall->GetOffset();

			ImGui::Text("Offset"); ImGui::SameLine();
			ImGui::InputFloat2("##Offset", (float*)&offset);

			wall->SetOffset(offset);

			if (Input::GetInstance()->GetKeyPress(eKeyCode::LCTRL))
			{
				if (Input::GetInstance()->GetKeyDown(eKeyCode::C))
				{
					mCaptureSize = mSize;
					mCaptureOffset = offset;
				}
				else if (Input::GetInstance()->GetKeyDown(eKeyCode::V))
				{
					Transform* tr = GetTarget()->GetComponent<Transform>();
					mBackupOffset = wall->GetOffset();
					mBackupSize = mSize;

					tr->SetPosition(mPosition);
					tr->SetRotation(mRotation);
					tr->SetScale(mScale);
					tr->SetSize(mCaptureSize);
					wall->SetOffset(mCaptureOffset);
					return;
				}
				if (Input::GetInstance()->GetKeyDown(eKeyCode::Z))
				{
					Transform* tr = GetTarget()->GetComponent<Transform>();

					tr->SetPosition(mPosition);
					tr->SetRotation(mRotation);
					tr->SetScale(mScale);
					tr->SetSize(mBackupSize);
					wall->SetOffset(mBackupOffset);
					return;
				}
			}
		}

		if (GetTarget())
		{
			Transform* tr = GetTarget()->GetComponent<Transform>();

			tr->SetPosition(mPosition);
			tr->SetRotation(mRotation);
			tr->SetScale(mScale);
			tr->SetSize(mSize);
		}
	}

}