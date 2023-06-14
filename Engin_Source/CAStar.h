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
		Vec(Math::Vector2 pos) : x((int)pos.x), y((int)pos.y) {};
		Vec() : x(0), y(0) {};

		bool operator == (Vec& vec)
		{
			if (this->x == vec.x && this->y == vec.y)
				return true;
			else
				return false;
		}

		void operator *=(int num)
		{
			this->x = num * this->x;
			this->y = num * this->y;
		}

		void operator =(const Vec& vec)
		{
			this->x = vec.x;
			this->y = vec.y;
		}

		Vec operator * (int num)
		{
			return Vec(this->x * num, this->y * num);
		}

		Vec operator * (float num)
		{
			return Vec(this->x * num, this->y * num);
		}
	};
	struct Node
	{
		Vec Pos;
		
		UINT Id;

		int Cost;
		int Heuristick;
		long long int Distance;

		Vec ParentIndex;

		class TileObject* Tile;

		Node(const Vec& pos, int g, int h, long long int f, Vec parent,  TileObject* tile)
			: Pos(pos), Id(0), Cost(g), Heuristick(h), Distance(f), ParentIndex(parent), Tile(tile) {};

		Node(const Vec& pos, int g, int h, long long int f, Vec parent)
			: Pos(pos), Id(0), Cost(g), Heuristick(h), Distance(f), ParentIndex(parent), Tile(nullptr) {};

		Node(const Vec& pos, int g, int h, long long int f)
			: Pos(pos), Id(0), Cost(g), Heuristick(h), Distance(f), ParentIndex(Vec{}), Tile(nullptr) {};

		Node(int& x, int& y, int& g, int& h, long long int& f, Vec parent, TileObject* tile)
			: Pos(x, y), Id(0), Cost(g), Heuristick(h), Distance(f), ParentIndex(parent), Tile(tile) {};

		Node(int& x, int& y, int& g, int& h, long long int& f, Vec parent)
			: Pos(x,y), Id(0), Cost(g), Heuristick(h), Distance(f), ParentIndex(parent), Tile(nullptr) {};

		Node(int& x, int& y, int& g, int& h, long long int& f)
			: Pos(x, y), Id(0), Cost(g), Heuristick(h), Distance(f), ParentIndex(Vec{}), Tile(nullptr) {};

		Node(const Node& node) 
			: Pos(node.Pos.x, node.Pos.y), Id(node.Id), Cost(node.Cost), Heuristick(node.Heuristick), Distance(node.Distance), ParentIndex(node.ParentIndex), Tile(node.Tile) {};

		Node() 
			: Pos(0, 0), Id(0), Cost(0), Heuristick(0), Distance(0), ParentIndex(Vec{}), Tile(nullptr) {}

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
	UINT GetHeuristick(int x, int y, Vec End);
	UINT GetHeuristick2(int x, int y);
	UINT GetHeuristick2(int x, int y, Vec End);
	UINT GetCost(int x, int y, int InG);
	UINT GetID(int x, int y);
	UINT GetID(int x, int y, int maxX, int maxY);

	void AddOpenList(int x, int y, bool diagonal = true);
	void CarveAddOpenList(int x, int y, Vec End, bool diagonal = true);

	void AddCloseList();
	void CarveAddCloseList();

	void RemoveOpenList(Node node);

	bool OnA_Star(Node& node, Vec& start, Vec& end, bool run = true);
	bool OnA_Star(Node& node, int x, int y, Vec& end, bool run = true);
	bool OnA_Star(Node& node, int x, int y,int endX, int endY, bool run = true);
	bool OnA_Star(std::pair<int, int> idx, Math::Vector2 endPos);
	void Compare(Node overlap);
	void CarveCompare(Node overlap, bool carve = true);
	Node DistanceList();

	void Result();
	void CarveTileAStar();
	void SaveCarvePosData();

	UINT GetMaxX() { return mMaxX; }
	UINT GetMaxY() { return mMaxY; }

	Vec& GetStart() { return mStart; }
	Vec& GetEnd() { return mEnd; }

	bool GetNodeEmpyt() { return mbNodeEmpty; }
	std::stack<Math::Vector2>& GetPosData() { return PosData; }
	Math::Vector2 GetCarvePos(Node curNode);

	bool IsRunning() { return mbRun; }
	void StopRun() { mbRun = false; }
	void ClearNode();

	Node* GetNextNode();
	void PopNode();

private:
	UINT mMaxX;
	UINT mMaxY;

	Vec mStart;
	Vec mEnd;
	Math::Vector2 mEndPos;
	
	unordered_map<UINT, Node> mOpenList;
	unordered_map<UINT, Node> mCloseList;

	priority_queue< Node, vector< Node>, cmp> mDistanceList;
	vector<Node> mResult;
	map<UINT, Node> mFindResult;

	std::vector<vector<TileObject*>> Tiles;
	vector<vector<TileCarveData>> mTilesCarveData;
	std::stack<Math::Vector2>PosData;

	Node mStartNode;
	Node mCurNode;

	Node mCarveCurNode;
	Node mCarveStartNode;
	Node mCarveEndNode;

	bool mbRun;
	bool mbNodeEmpty;
};

typedef vector<vector<Math::Vector2>> PositionDataIter;
typedef vector<AStar::Node> ResultIter;