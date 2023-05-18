#pragma once
#include "GuiWidget.h"

namespace gui
{
	class TreeWidget : public Widget
	{
	public:
		struct Node : public guiEntity
		{
		public:
			Node();
			~Node();

			void Update();

			void SetData(void* data) { mData = data; }
			void SetStem(bool enable) { mbStem = enable; }

			void AddNode(Node* node);
			const std::vector<Node*>& GetChilds() { return mChilds; }


		public:
			TreeWidget* mTreeWidget;
			void* mData;

			Node* mParent;
			std::vector<Node*> mChilds;

			bool mbStem;
			bool mbSelected;
		};

	public:
		TreeWidget();
		virtual ~TreeWidget();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Close() override;

		Node* AddNode(Node* parent, const std::string& name, void* data, bool stem = false);
		void Clear();
		void SetDummyRoot(bool enable) { mbDummyRootUse = enable; }
		void SetSelectNode(Node* node);

		void SetEvent(Widget* widget, std::function<void(void* data)> func)
		{
			mEventWidget = widget;
			mEvent = std::move(func);
		}

	private:
		Node* mRoot;

		bool mbDummyRootUse;
		Node* mSelectNode;

		Widget* mEventWidget;
		std::function<void(void* data)> mEvent;
	};
}

