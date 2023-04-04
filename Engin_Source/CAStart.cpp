#include "CAStart.h"
#include "CWorldManager.h"

AStar::AStar()
	: Component(eComponentType::AStart)
	, mMaxX(0)
	, mMaxY(0)
	, mStart(-1, -1)
	, mEnd(-1, -1)
	, mbRun(false)
	, mCurNode{}
{
}

AStar::~AStar()
{
	ClearNode();
}

void AStar::Initalize()
{
}

void AStar::Update()
{
}

void AStar::FixedUpdate()
{
	if (!mbRun)
		return;

	while (1)
	{
		if (WorldManager::GetInstance()->GetTileNum(mCurNode.Pos.x, mCurNode.Pos.y) == 2)
		{
			
			Result();
			return;
		}

		// ↗ ↘ ↙ ↖
		int arr1X[4] = { 1, 1, -1, -1 };
		int arr1Y[4] = { -1, 1, 1, -1 };

		for (int i = 0; i < 4; ++i) { AddOpenList(mCurNode.Pos.x + arr1X[i], mCurNode.Pos.y + arr1Y[i]); }

		// → ↓ ← ↑
		int arr2X[4] = { 1, 0, -1, 0 };
		int arr2Y[4] = { 0, 1, 0, -1 };

		for (int i = 0; i < 4; ++i) { AddOpenList(mCurNode.Pos.x + arr2X[i], mCurNode.Pos.y + arr2Y[i], false); }

		AddCloseList();
	}
}

void AStar::Render()
{
}

UINT AStar::GetHeuristick(int x, int y)
{
	//int ix = abs(x - mEnd.X);
	//int iy = abs(y - mEnd.Y);

	//int tem = abs(ix - iy);
	//int weight = 1;

	//// 상황에 따른 가중치 알고리즘 추가하면
	//// 조금 더 정교한 탐색이 가능해진다
	///*if (tem == 0)
	//    weight = 14;
	//else
	//    weight = 10;*/

	//return (int)(weight * (std::sqrt(std::pow(x - mEnd.X, 2) + std::pow(y - mEnd.Y, 2))));

	int ix = abs(x - mEnd.x);
	int iy = abs(y - mEnd.y);

	int tem = abs(ix - iy);
	int weight = 1;
	if (tem == 0)
		weight = 14;
	else
		weight = 10;

	UINT H = (ix + iy) * 10;
	return H + weight;
}

UINT AStar::GetCost(int x, int y, int InG)
{
	int ix = abs(x - mEnd.x);
	int iy = abs(y - mEnd.y);

	int tem = abs(ix + iy);

	int outG = InG;
	if (tem > 1)
		outG += 14;
	else
		outG += 10;

	return outG;
}

UINT AStar::GetID(int x, int y)
{
	UINT id = (y * mMaxY) + (x % mMaxX);
	return id;
}

void AStar::AddOpenList(int x, int y, bool diagonal)
{
	Node node = {};

	node.Pos.x = x;
	node.Pos.y = y;

	node.Id = GetID(node.Pos.x, node.Pos.y);

	// 맵 범위 밖 타일인지확인
	if (node.Pos.x  < 0 || node.Pos.x  > mMaxX - 1)
		return;

	if (node.Pos.y < 0 || node.Pos.y > mMaxY - 1)
		return;

	// 해당타일이 장애물이 아닌지 확인
	if (WorldManager::GetInstance()->GetTileNum(node.Pos.x, node.Pos.y) == 3)
		return;

	// 대각 선이동의 경우 해당타일의
	// 직각이 되는 타일이 장애물이면 리턴
	if (diagonal)
	{
		if (WorldManager::GetInstance()->GetTileNum(mCurNode.Pos.x, node.Pos.y) == 3)
			return;

		if (WorldManager::GetInstance()->GetTileNum(node.Pos.x, mCurNode.Pos.y) == 3)
			return;
	}

	// 닫힌 목록에 있는 노드인지 탐색
	unordered_map<UINT, Node>::iterator iter;
	iter = mCloseList.find(node.Id);

	if (iter != mCloseList.end())
		return;

	// 오픈노드에 있는 노드인지 탐색
	iter = mOpenList.find(node.Id);
	if (iter != mOpenList.end())
	{
		if (diagonal)
			return;

		// G 값 비교후 들어있는 
		// 타일 데이터 수정 G, F
		Compare(iter->second);
		return;
	}

	int count = abs(mCurNode.Pos.x - node.Pos.x) + abs(mCurNode.Pos.y - node.Pos.y);
	if (count > 1)
		node.Cost = 14 + mCurNode.Cost;
	else
		node.Cost = 10 + mCurNode.Cost;

	node.Heuristick = GetHeuristick(node.Pos.x, node.Pos.y);
	node.Distance = node.GetDistance();

	node.ParentIndex = mCurNode.Pos;

	Node pushNode = Node{node.Pos.x, node.Pos.y, node.Cost, node.Heuristick, node.Distance, node.ParentIndex };
	pushNode.Id = node.Id;

	mOpenList.insert(make_pair(pushNode.Id, pushNode));
	mDistanceList.emplace(pushNode);
}

void AStar::AddCloseList()
{
	mCurNode = DistanceList();
	mCloseList.emplace(std::make_pair(mCurNode.Id, mCurNode));
}

void AStar::RemoveOpenList(Node node)
{
	unordered_map<UINT, Node>::iterator iter;
	iter = mOpenList.find(node.Id);
	if (iter != mOpenList.end())
		mOpenList.erase(iter);
}

bool AStar::OnA_Star(Node& node, Vec& start, Vec& end, bool run)
{
	if (mbRun)
		return false;

	ClearNode();

	mMaxX = WorldManager::GetInstance()->GetScale();
	mMaxY = WorldManager::GetInstance()->GetScale();
	
	//SetIndex
	mStart = start;
	mEnd = end;
	mbRun = true;

	// SetNode
	mCurNode = Node{ node.Pos.x, node.Pos.y, node.Cost, node.Heuristick, node.Distance, node.ParentIndex };

	mCurNode.Id = (mCurNode.Pos.y * mMaxY) + (mCurNode.Pos.x % mMaxX);
	mCurNode.Cost = 0;
	mCurNode.Heuristick = GetHeuristick(node.Pos.x, node.Pos.y);
	mCurNode.Distance = mCurNode.GetDistance();

	mCloseList.emplace(make_pair(mCurNode.Id, mCurNode));
	return true;
}

void AStar::Compare(Node duplication)
{
	UINT g = mCurNode.Cost;

	int x = abs(mCurNode.Pos.x - duplication.Pos.x);
	int y = abs(mCurNode.Pos.y - duplication.Pos.y);

	if (x + y > 1)
		g = mCurNode.Cost += 14;
	else
		g = mCurNode.Cost += 10;

	if (duplication.Cost > g)
	{
		duplication.ParentIndex = mCurNode.Pos;

		duplication.Cost = g;
		duplication.Distance = duplication.Cost + duplication.Heuristick;
	}

	unordered_map<UINT, Node>::iterator iter;
	vector<Node> Nodetemp;
	iter = mOpenList.find(duplication.Id);

	Node node = iter->second;
	mOpenList.erase(iter);

	while (!mDistanceList.empty())
	{
		Node temp = mDistanceList.top();
		mDistanceList.pop();

		if (node.Id == temp.Id)
			break;

		Nodetemp.emplace_back(temp);
	}

	mDistanceList.emplace(node);
	for (int i = 0; i < Nodetemp.size(); ++i)
	{
		mDistanceList.emplace(Nodetemp[i]);
	}
}

AStar::Node AStar::DistanceList()
{
	/*std::sort(open.begin(), open.end());

	vector<Node*>::iterator iter;
	iter = open.begin();

	Node* node = *(iter);

	for (int i = 0; i < open.size(); ++i)
	{
		if (node->FValue > open[i]->FValue)
			break;

		if (node->F() == open[i]->F())
		{
			if (node->HValue < open[i]->HValue)
				continue;
		}

		node = open[i];
	}

	for (vector<Node*>::iterator iter = open.begin(); iter != open.end(); iter++)
	{
		if (*iter == node)
		{
			open.erase(iter);
			break;
		}
	}

	return node;*/

	/*unordered_map<UINT, Node*>::iterator iter;
	long long int min = 999999999999999999;
	int index = -1;
	Node* outNode = nullptr;

	iter = mOpenList.begin();

	for (; iter != mOpenList.end(); )
	{

		if ((iter->second->FValue <= min))
		{

			if (iter->second->FValue == min)
			{
				unordered_map<UINT, Node*>::iterator miniter;
				miniter = mOpenList.find(index);
				if (miniter == mOpenList.end())
					continue;

				if (miniter->second->HValue > iter->second->HValue)
				{
					min = iter->second->FValue;
					index = iter->first;

					outNode = iter->second;
					iter++;
					continue;
				}
			}

			min = iter->second->FValue;
			index = iter->first;

			outNode = iter->second;
		}
		iter++;
	}

	if (!outNode)
		int a = 0;

	return outNode;*/

	Node node = mDistanceList.top();
	mDistanceList.pop();
	RemoveOpenList(node);

	return node;
}

void AStar::Result()
{
	while (!mResult.empty())
	{
		Node node = mResult.top();
		mResult.pop();
	}

	unordered_map<UINT, Node>::iterator iter;
	UINT count = 0;

	Node node = mCurNode;
	while (1)
	{
		if ((mCurNode.Pos.x == mStart.x && mCurNode.Pos.y == mStart.y))
		{
			WorldManager::GetInstance()->SetEndIndex(node.Pos.x, node.Pos.y);

			mbRun = false;
			return;
		}

		if (count == 100000)
			return;

		mResult.push(mCurNode);

		int id = (mCurNode.ParentIndex.y * mMaxY) + (mCurNode.ParentIndex.x % mMaxX);
		iter = mCloseList.find(id);
		if (iter == mCloseList.end())
			continue;

		mCurNode = iter->second;
		count++;
	}
}

void AStar::ClearNode()
{
	/*unordered_map<UINT, Node>::iterator iter;
	for (iter = mOpenList.begin();
		iter != mOpenList.end();
		iter++)
	{
		delete iter.second;
		iter.second = nullptr;
	}

	for (iter = mCloseList.begin();
		iter != mCloseList.end();
		iter++)
	{
		delete iter.second;
		iter.second = nullptr;
	}*/

	mOpenList.clear();
	mCloseList.clear();

	while (!mResult.empty()) { mResult.pop(); }
	while (!mDistanceList.empty()) { mDistanceList.pop(); }
}

AStar::Node* AStar::GetNextNode()
{
	if (mResult.empty())
	{
		return nullptr;
	}
	else
	{
		Node* node = &(mResult.top());
		mResult.pop();

		return node;
	}
	return nullptr;
}

