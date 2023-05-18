#pragma once
#include "GuiWidget.h"

namespace gui
{
	class ListWidget : public Widget
	{
	public:
		ListWidget();
		virtual ~ListWidget();

		virtual void Update() override;
		virtual void Close() override;

		void SetEvent(Widget* widget, std::function<void(std::string key)> evnet)
		{
			mEventWidget = widget;
			mEvent = std::move(evnet);
		}

		void SetItemList(const std::vector<std::wstring>& list);
		void SetItemList(const std::vector<std::string>& list);
	private:
		std::vector<std::string> mResourceLIst;
		int mListIndex;

		Widget* mEventWidget;
		std::function<void(std::string key)> mEvent;
	};
}

