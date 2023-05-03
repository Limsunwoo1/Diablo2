#include "GuiWidget.h"

namespace gui
{
	Widget::Widget()
		: mState(eState::Active)
		, mParent(nullptr)
		, mChilds()
	{

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

		// 최상위 부모일때
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
			ImGui::BeginChild(GetName().c_str());
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