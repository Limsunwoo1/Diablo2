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
		Vec() {};
	};
	struct Node
	{
		Vec Pos;
		
		UINT Id;

		int Cost;
		int Heuritick;
		long long int Distance;

		Vec ParentIndex;

		Node(const Vec& pos, int g, int h, long long int f, Vec parent)
			: Pos(pos), Id(0), Cost(g), Heuritick(h), Distance(f), ParentIndex(parent) {};

		Node(int& x, int& y, int& g, int& h, long long int& f, Vec parent)
			: Pos(x,y), Id(0), Cost(g), Heuritick(h), Distance(f), ParentIndex(parent) {};

		Node(const Node& node) 
			: Pos(node.Pos.x, node.Pos.y), Id(node.Id), Cost(node.Cost), Heuritick(node.Heuritick), Distance(node.Distance), ParentIndex(node.ParentIndex) {};

		Node(){};

		/*void operator=(const Node& node)
		{
			this->V = node.V;
			this->GValue = node.GValue;
			this->HValue = node.HValue;
			this->FValue = node.FValue;

			this->PartenNode = node.PartenNode;
		}*/
		bool operator<(const Node& other) const 
		{   // 우선순위 큐에서 사용될 연산자
			return Distance > other.Distance;
		}

		long long int GetDistance() { return Cost + Heuritick; }
	};
	struct cmp
	{
		bool operator()(Node& a,  Node& b)
		{
			if (a.GetDistance() == b.GetDistance())
				return a.Heuritick > b.Heuritick;

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

	UINT GetHeuritick(int x, int y);
	UINT GetCost(int x, int y, int InG);
	UINT GetID(int x, int y);

	void AddOpenList(int x, int y, bool diagonal = true);
	void AddCloseList();

	void RemoveOpenList(Node node);

	void OnA_Star(Node& node, Vec& start, Vec& end, bool run = true);
	void Compare(Node overlap);
	Node DistanceList();

	const stack<Node>& Result();

	UINT GetMaxX() { return mMaxX; }
	UINT GetMaxY() { return mMaxY; }

	Vec& GetStart() { return mStart; }
	Vec& GetEnd() { return mEnd; }

	bool IsRunning() { return mbRun; }
	void StopRun() { mbRun = false; }
	void ClearNode();

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
};

