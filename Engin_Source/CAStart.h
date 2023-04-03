#pragma once
#include "CComponent.h"

using namespace std;

class AStar : public Component
{
public:
	struct Vec
	{
		int X;
		int Y;

		Vec(int x, int y) : X(x), Y(y) {};
		Vec(Vec& pos) : X(pos.X), Y(pos.Y) {};
		Vec() {};
	};
	struct Node
	{
		Vec V;
		
		UINT Id;

		int GValue;
		int HValue;
		long long int FValue;

		Node* PartenNode;

		Node(Vec pos, int g, int h, long long int f, Node* parent)
			: V(pos), Id(0), GValue(g), HValue(h), FValue(f), PartenNode(parent) {};

		Node(int x, int y, int g, int h, long long int f, Node* parent)
			: V(x,y), Id(0), GValue(g), HValue(h), FValue(f), PartenNode(parent) {};

		Node(Node& node) 
			: V(node.V), Id(node.Id), GValue(node.GValue), HValue(node.HValue), FValue(node.FValue), PartenNode(node.PartenNode) {};
		Node(){};

		void operator=(Node& node)
		{
			this->V = node.V;
			this->GValue = node.GValue;
			this->HValue = node.HValue;
			this->FValue = node.FValue;

			this->PartenNode = node.PartenNode;
		}
		bool operator<(const Node& other) const 
		{   // 우선순위 큐에서 사용될 연산자
			return FValue> other.FValue;
		}

		long long int F() { return GValue + HValue; }
	};
	struct cmp
	{
		bool operator()(Node* a,  Node* b)
		{
			if (a->F() == b->F())
				return a->HValue > b->HValue;

			return a->F() > b->F();
		}
	};
public:
	AStar();
	virtual ~AStar();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	UINT H(int x, int y);
	UINT G(int x, int y, int InG);
	//long long int F(int g, int H);
	UINT ID(int x, int y);

	void PushOpenList(int x, int y, bool diagonal = true);
	Node* PushCloseList();

	void DeleteOpenList(Node* node);

	void OnA_Star(Node& node, Vec start, Vec end, bool run = true);
	void Test(Node* duplication);
	Node* LowFValue();

	stack<Node*>& Result();

	UINT GetMaxX() { return mMaxX; }
	UINT GetMaxY() { return mMaxY; }

	Vec& GetStart() { return mStart; }
	Vec& GetEnd() { return mEnd; }

	bool IsRunning() { return mbRun; }

	void StopRun() { mbRun = false; }

	void Clear();
	void ClearNode();

	bool FSort(std::pair<UINT, Node*>& a, std::pair<UINT, Node*>& b);

private:
	UINT mMaxX;
	UINT mMaxY;

	Vec mStart;
	Vec mEnd;
	
	unordered_map<UINT, Node*> mOpenList;
	unordered_map<UINT, Node*> mCloseList;
	vector<Node*> open;

	priority_queue< Node*, vector< Node*>, cmp> mOpenlistF;

	stack<Node*> mResult;

	Node* mCurNode;

	bool mbRun;
};

