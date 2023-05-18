#include "GuiTreeWidget.h"


namespace gui
{
	// Node
	TreeWidget::Node::Node()
		: mTreeWidget(nullptr)
		, mData(nullptr)
		, mParent(nullptr)
		, mChilds {}
		, mbStem(false)
		, mbSelected(false)
	{
	}

	TreeWidget::Node::~Node()
	{
		for (Node* child : mChilds)
		{
			if (child == nullptr)
				continue;

			delete child;
			child = nullptr;
		}
	}

	void TreeWidget::Node::Update()
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

	void TreeWidget::Node::AddNode(Node* node)
	{
		node->mParent = this;
		mChilds.emplace_back(node);
	}

	// TreeWidget
	TreeWidget::TreeWidget()
		: mRoot(nullptr)
		, mbDummyRootUse(false)
		, mSelectNode(nullptr)
		, mEventWidget(nullptr)
		, mEvent(nullptr)
	{

	}
	TreeWidget::~TreeWidget()
	{
		delete mRoot;
		mRoot = nullptr;
	}
	void TreeWidget::FixedUpdate()
	{
	}
	void TreeWidget::Update()
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
	void TreeWidget::LateUpdate()
	{
	}

	TreeWidget::Node* TreeWidget::AddNode(TreeWidget::Node* parent, const std::string& name, void* data, bool stem)
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
	void TreeWidget::Close()
	{
	}

	void TreeWidget::Clear()
	{
		if (mRoot != nullptr)
		{
			delete mRoot;
			mRoot = nullptr;
		}
	}
	void TreeWidget::SetSelectNode(TreeWidget::Node* node)
	{
		if (nullptr != mSelectNode)
			mSelectNode->mbSelected = false;

		mSelectNode = node;
		mSelectNode->mbSelected = false;

		if (mSelectNode && mEvent)
		{
			mEvent(mSelectNode->mData);
		}
	}
}