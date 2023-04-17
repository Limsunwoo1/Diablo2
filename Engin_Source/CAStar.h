#pragma once
#include "CComponent.h"

using namespace std;

class AStar : public Component
{
public:
	struct Vec
	{
		int x;
		int y;

		Vec(int x, int y) : x(x), y(y) {};
		Vec(const Vec& pos) : x(pos.x), y(pos.y) {};
		Vec() : x(0), y(0) {};
	};
	struct Node
	{
		Vec Pos;
		
		UINT Id;

		int Cost;
		int Heuristick;
		long long int Distance;

		Vec ParentIndex;

		Node(const Vec& pos, int g, int h, long long int f, Vec parent)
			: Pos(pos), Id(0), Cost(g), Heuristick(h), Distance(f), ParentIndex(parent) {};

		Node(const Vec& pos, int g, int h, long long int f)
			: Pos(pos), Id(0), Cost(g), Heuristick(h), Distance(f), ParentIndex(Vec{}) {};

		Node(int& x, int& y, int& g, int& h, long long int& f, Vec parent)
			: Pos(x,y), Id(0), Cost(g), Heuristick(h), Distance(f), ParentIndex(parent) {};

		Node(int& x, int& y, int& g, int& h, long long int& f)
			: Pos(x, y), Id(0), Cost(g), Heuristick(h), Distance(f), ParentIndex(Vec{}) {};

		Node(const Node& node) 
			: Pos(node.Pos.x, node.Pos.y), Id(node.Id), Cost(node.Cost), Heuristick(node.Heuristick), Distance(node.Distance), ParentIndex(node.ParentIndex) {};

		Node() 
			: Pos(0, 0), Id(0), Cost(0), Heuristick(0), Distance(0), ParentIndex(Vec{}) {};

		/*void operator=(const Node& node)
		{
			this->V = node.V;
			this->GValue = node.GValue;
			this->HValue = node.HValue;
			this->FValue = node.FValue;

			this->PartenNode = node.PartenNode;
		}*/
		bool operator<(const Node& other) const 
		{   // �켱���� ť���� ���� ������
			return Distance > other.Distance;
		}

		long long int GetDistance() { return Cost + Heuristick; }
	};
	struct cmp
	{
		bool operator()(Node& a,  Node& b)
		{
			if (a.GetDistance() == b.GetDistance())
				return a.Heuristick > b.Heuristick;

			return a.GetDistance() > b.GetDistance();
		}
	};
public:
	AStar();
	virtual ~AStar();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	UINT GetHeuristick(int x, int y);
	UINT GetHeuristick2(int x, int y);
	UINT GetCost(int x, int y, int InG);
	UINT GetID(int x, int y);

	void AddOpenList(int x, int y, bool diagonal = true);
	void AddCloseList();

	void RemoveOpenList(Node node);

	bool OnA_Star(Node& node, Vec& start, Vec& end, bool run = true);
	bool OnA_Star(Node& node, int x, int y, Vec& end, bool run = true);
	bool OnA_Star(Node& node, int x, int y,int endX, int endY, bool run = true);
	void Compare(Node overlap);
	Node DistanceList();

	void Result();

	UINT GetMaxX() { return mMaxX; }
	UINT GetMaxY() { return mMaxY; }

	Vec& GetStart() { return mStart; }
	Vec& GetEnd() { return mEnd; }

	bool GetNodeEmpyt() { return mbNodeEmpty; }
	stack<Node> GetNodes() { return mResult; }

	bool IsRunning() { return mbRun; }
	void StopRun() { mbRun = false; }
	void ClearNode();

	Node* GetNextNode();

private:
	UINT mMaxX;
	UINT mMaxY;

	Vec mStart;
	Vec mEnd;
	
	unordered_map<UINT, Node> mOpenList;
	unordered_map<UINT, Node> mCloseList;

	priority_queue< Node, vector< Node>, cmp> mDistanceList;
	stack<Node> mResult;

	Node mCurNode;

	bool mbRun;
	bool mbNodeEmpty;
};

