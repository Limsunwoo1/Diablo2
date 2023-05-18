#include "GuiListWidget.h"



namespace gui
{
	ListWidget::ListWidget()
		: Widget()
		, mListIndex(-1)
	{
		SetName("ListWidget");
		SetState(eState::Paused);
		SetSize(ImVec2(200.f, 200.f));
	}
	ListWidget::~ListWidget()
	{
	}
	void ListWidget::Update()
	{
		if (ImGui::BeginListBox("##ListBox", GetSize()))
		{
			WindowFocus(true);
			for (int i = 0; i < mResourceLIst.size(); ++i)
			{
				bool selectable = (mListIndex == i);
				if (ImGui::Selectable(mResourceLIst[i].c_str(), &selectable))
					mListIndex = i;

				if (selectable)
				{
					ImGui::SetItemDefaultFocus();
				}

				// 해당 아이템이 더블 클릭 되었 다면
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (mEventWidget && mEvent)
						mEvent(mResourceLIst[i].c_str());

					Close();
				}
			}
			ImGui::EndListBox();
		}
	}
	void ListWidget::Close()
	{
		SetState(eState::Paused);
		mResourceLIst.clear();

		WindowFocus(false);
	}

	void ListWidget::SetItemList(const std::vector<std::wstring>& list)
	{
		for (auto wName : list)
		{
			std::string name(wName.begin(), wName.end());
			mResourceLIst.emplace_back(name);
		}
	}

	void ListWidget::SetItemList(const std::vector<std::string>& list)
	{
		mResourceLIst = list;
	}

}