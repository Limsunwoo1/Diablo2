#pragma once
#include "..//Engin_Source/CGameObject.h"
#include "..//Engin_Source/CEntity.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace gui
{
	class Widget : public gui::Entity
	{
	public:
		enum class eState
		{
			Paused,
			Active,
			Dead,
		};

	public:
		Widget();
		virtual ~Widget();

		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();

		virtual void Close() {};
		
		void AddWidget(Widget* widget);

		void SetState(eState state) { mState = state; }
		eState GetState() { return mState; }

		void SetParent(Widget* parent) { mParent = parent; }
		Widget* GetParent() { return mParent; }

		void SetSize(ImVec2 size) { mSize = size; }
		ImVec2 GetSize() { return mSize; }
		
	protected:
		ImGuiWindowFlags mWindow_flags;

	private:
		eState mState;
		Widget* mParent;
		std::vector<Widget*> mChilds;
		ImVec2 mSize;
	};
}

