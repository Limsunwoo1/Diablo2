#include "GuiTreeWidget.h"


namespace gui
{
	// Node
	GuiTreeWidget::Node::Node()
		: mTreeWidget(nullptr)
		, mData(nullptr)
		, mParent(nullptr)
		, mChilds {}
		, mbStem(false)
		, mbSelected(false)
	{
	}

	GuiTreeWidget::Node::~Node()
	{
		for (Node* child : mChilds)
		{
			if (child == nullptr)
				continue;

			delete child;
			child = nullptr;
		}
	}

	void GuiTreeWidget::Node::Update()
	{
		// ImGui TreeNodeFlags_::ImGuiTreeNodeFlafs_Framed;

		int flag = 0;

		if (mbStem)
			flag |= ImGuiTreeNodeFlags_Framed;
		if (mbSelected)
			flag |= ImGuiTreeNodeFlags_Selected;
		if (mChilds.empty())
			flag |= ImGuiTreeNodeFlags_Leaf;

		if (mbStem && mChilds.empty())
			SetName("\t" + GetName());

		if (ImGui::TreeNodeEx(GetName().c_str(), flag))
		{
			if (!mbStem && ImGui::IsItemHovered(0) && ImGui::IsMouseClicked(0))
			{
				mTreeWidget->SetSelectNode(this);
			}

			for (Node* node : mChilds)
				node->Update();

			ImGui::TreePop();
		}
	}

	void GuiTreeWidget::Node::AddNode(Node* node)
	{
		node->mParent = this;
		mChilds.emplace_back(node);
	}

	// TreeWidget
	GuiTreeWidget::GuiTreeWidget()
		: mRoot(nullptr)
		, mbDummyRootUse(false)
		, mSelectNode(nullptr)
		, mEventWidget(nullptr)
		, mEvent(nullptr)
	{

	}
	GuiTreeWidget::~GuiTreeWidget()
	{
		delete mRoot;
		mRoot = nullptr;
	}
	void GuiTreeWidget::FixedUpdate()
	{
	}
	void GuiTreeWidget::Update()
	{
		if (mRoot == nullptr)
			return;

		// mRoot->Update();

		if (!mbDummyRootUse)
		{
			mRoot->Update();
		}
		else
		{
			const std::vector<Node*>& childs = mRoot->GetChilds();

			for (Node* child : childs)
			{
				child->Update();
			}
		}
	}
	void GuiTreeWidget::LateUpdate()
	{
	}

	GuiTreeWidget::Node* GuiTreeWidget::AddNode(GuiTreeWidget::Node* parent, const std::string& name, void* data, bool stem)
	{
		Node* node = new Node();
		node->SetName(name);
		node->SetData(data);
		node->SetStem(stem);
		node->mTreeWidget = this;

		if (parent == nullptr)
			mRoot = node;
		else
			parent->AddNode(node);

		return node;
	}
	void GuiTreeWidget::Close()
	{
	}

	void GuiTreeWidget::Clear()
	{
		if (mRoot != nullptr)
		{
			delete mRoot;
			mRoot = nullptr;
		}
	}
	void GuiTreeWidget::SetSelectNode(GuiTreeWidget::Node* node)
	{
		if (nullptr != mSelectNode)
			mSelectNode->mbSelected = false;

		mSelectNode = node;
		mSelectNode->mbSelected = false;

		if (mSelectNode && mEvent)
		{

		}
	}
}