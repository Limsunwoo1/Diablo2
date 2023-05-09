#include "GuiWidget.h"

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

		for (Widget* child : mChilds)
		{
			child->FixedUpdate();
		}
	}

	void Widget::Update()
	{
		if (mState != eState::Active)
			return;

		for (Widget* child : mChilds)
		{
			child->Update();
		}
	}

	void Widget::LateUpdate()
	{
		if (mState != eState::Active)
			return;

		for (Widget* child : mChilds)
		{
			child->LateUpdate();
		}
	}

	void Widget::Render()
	{
		if (mState != eState::Active)
			return;

		// �ֻ��� �θ��϶�
		if (mParent == nullptr)
		{
			bool open = (bool)GetState();
			FixedUpdate();
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
			// ����� �߰��� ����Ѵ�
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
}