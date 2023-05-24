#include "GuiWidget.h"
#include "GuiTexture2D.h"

namespace gui
{
	Widget::Widget()
		: mState(eState::Active)
		, mParent(nullptr)
		, mChilds{}
		, mWindow_flags(ImGuiWindowFlags_None)
	{
		SetName("Widget");
	}

	Widget::~Widget()
	{

	}

	void Widget::FixedUpdate()
	{
		if (mState != eState::Active)
			return;

		/*for (Widget* child : mChilds)
		{
			child->FixedUpdate();
		}*/
	}

	void Widget::Update()
	{
		if (mState != eState::Active)
			return;

		/*for (Widget* child : mChilds)
		{
			child->Update();
		}*/
	}

	void Widget::LateUpdate()
	{
		if (mState != eState::Active)
			return;

		/*for (Widget* child : mChilds)
		{
			child->LateUpdate();
		}*/
	}

	void Widget::Render()
	{
		if (mState != eState::Active)
			return;

		// 최상위 부모일때
		if (mParent == nullptr)
		{
			bool open = (bool)GetState();
			FixedUpdate();

			if(dynamic_cast<guiTexture2D*>(this) != nullptr)
				ImGui::Begin(GetName().c_str(), &open, ImGuiWindowFlags_NoTitleBar);
			else
				ImGui::Begin(GetName().c_str(), &open);

			Update();

			for (Widget* child : mChilds)
			{
				child->Render();
			}
			LateUpdate();

			ImGui::End();
		}
		else
		{
			FixedUpdate();
			// 사이즈를 추가해 줘야한다
			ImGui::BeginChild(GetName().c_str(), mSize);
			Update();

			for (Widget* child : mChilds)
			{
				child->Render();
			}
			LateUpdate();

			ImGui::EndChild();
		}
	}

	void Widget::AddWidget(Widget* widget)
	{
		mChilds.push_back(widget);
		widget->SetParent(this);
	}
	void Widget::WindowFocus(bool enable)
	{
		if (enable == true)
			ImGui::SetWindowFocus(GetName().c_str());
		else
			ImGui::SetWindowFocus(nullptr);
	}
}