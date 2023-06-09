#include "CAStar.h"
#include "CWorldManager.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CPlayerScript.h"
#include "CTileObject.h"
#include "CWorldManager.h"
#include "CInput.h"
#include "CObjectManager.h"

AStar::AStar()
	: Component(eComponentType::AStar)
	, mMaxX(0)
	, mMaxY(0)
	, mStart(-1, -1)
	, mEnd(-1, -1)
	, mbRun(false)
	, mbNodeEmpty(true)
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
	if (!mbRun)
		return;

	Tiles = WorldManager::GetInstance()->DropWordTileData();

	std::pair<int, int> StartTileIdx;
	std::pair<int, int> EndTileIdx;
	std::pair<int, int> CurTileIdx;

	while (1)
	{
		if (mCurNode.Pos == mEnd)
		{
			// 큰타일에서 작은타일 길찾기 시행
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

void AStar::FixedUpdate()
{

}

void AStar::Render()
{

}

UINT AStar::GetHeuristick(int x, int y)
{
	int ix = abs(x - mEnd.x);
	int iy = abs(y - mEnd.y);

	int tem = abs(ix - iy);
	int weight = 1;
	if (tem == 0)
		weight = 14;
	else
		weight = 10;

	UINT H = (ix + iy) * 10;
	return H;
}

UINT AStar::GetHeuristick2(int x, int y)
{
	int ix = abs(x - mEnd.x);
	int iy = abs(y - mEnd.y);

	int weight = mCloseList.size();
	// 상황에 따른 가중치 알고리즘 추가하면
	// 조금 더 정교한 탐색이 가능해진다
	/*if (tem == 0)
		weight = 14;
	else
		weight = 10;*/

	return (int)(1 * (std::sqrt(std::pow(x - mEnd.x, 2) + std::pow(y - mEnd.y, 2))));
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

	// 포즈가 현재 화면상인덱스 임
	// 시작 노드의 idx 를 변환해줘야함
	node.Pos.x = x;
	node.Pos.y = y;

	node.Id = GetID(node.Pos.x, node.Pos.y);
	node.Tile = Tiles[x][y];

	//test 
	//CurNode

	// 맵 범위 밖 타일인지확인
	if (y < 0 || y >= Tiles.size())
		return;

	if (x < 0 || x >= Tiles[y].size())
		return;


	// 해당타일이 장애물이 아닌지 확인
	if (Tiles[x][y] == nullptr)
		return;

	if (Tiles[x][y]->GetPass() == false)
		return;

	// 닫힌 목록에 있는 노드인지 탐색
	unordered_map<UINT, Node>::iterator iter;
	iter = mCloseList.find(node.Id);

	if (iter != mCloseList.end())
		return;

	// 오픈노드에 있는 노드인지 탐색
	iter = mOpenList.find(node.Id);
	if (iter != mOpenList.end())
	{
		/*if (diagonal)
			return;*/

		// G 값 비교후 들어있는 
		// 타일 데이터 수정 G, F
		Compare(iter->second);
		return;
	}

	// 대각 선이동의 경우 해당타일의
	// 직각이 되는 타일이 장애물이면 리턴
	if (diagonal)
	{
		int diffX = mCurNode.Pos.x - x;
		int diffY = mCurNode.Pos.y - y;

		bool check = false;
		if (diffX < 0 && y > 0)
			if (mCurNode.Tile->GetArr()[3] != 0 || node.Tile->GetArr()[0] != 0) check = true;
		else if (diffX < 0 && diffY < 0)
			if (mCurNode.Tile->GetArr()[1] != 0 || node.Tile->GetArr()[2] != 0) check = true;
		else if (diffX > 0 && diffY < 0)
			if (mCurNode.Tile->GetArr()[0] != 0 || node.Tile->GetArr()[3] != 0) check = true;
		else if (diffX > 0 && diffY > 0)
			if (mCurNode.Tile->GetArr()[2] != 0 || node.Tile->GetArr()[1] != 0) check = true;

		if (check)
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

	Node pushNode = Node(node.Pos.x, node.Pos.y, node.Cost, node.Heuristick, node.Distance, node.ParentIndex, node.Tile);
	pushNode.Id = node.Id;

	mOpenList.insert(make_pair(node.Id, node));
	mDistanceList.emplace(node);
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

	if (end.x == node.Pos.x && end.y == node.Pos.y)
		return false;

	ClearNode();

	Tiles = WorldManager::GetInstance()->DropWordTileData();
	mMaxX = Tiles.size();
	mMaxY = Tiles.size();

	//SetIndex
	mStart = start;
	mEnd = end;
	mbRun = true;

	// SetNode
	mCurNode = Node{ node.Pos.x, node.Pos.y, node.Cost, node.Heuristick, node.Distance, node.ParentIndex, node.Tile };

	mCurNode.Id = (mCurNode.Pos.y * mMaxY) + (mCurNode.Pos.x % mMaxX);
	mCurNode.Cost = 0;
	mCurNode.Heuristick = GetHeuristick(node.Pos.x, node.Pos.y);
	mCurNode.Distance = mCurNode.GetDistance();

	mCloseList.emplace(make_pair(mCurNode.Id, mCurNode));
	return true;
}

bool AStar::OnA_Star(Node& node, int x, int y, Vec& end, bool run)
{
	if (mbRun)
		return false;

	if (end.x == node.Pos.x && end.y == node.Pos.y)
		return false;

	ClearNode();

	mMaxX = WorldManager::GetInstance()->GetScale();
	mMaxY = WorldManager::GetInstance()->GetScale();

	//SetIndex
	mStart = Vec(x, y);
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

bool AStar::OnA_Star(Node& node, int x, int y, int endX, int endY, bool run)
{
	if (mbRun)
		return false;

	if (endX == node.Pos.x && endY == node.Pos.y)
		return false;

	ClearNode();

	mMaxX = WorldManager::GetInstance()->GetScale();
	mMaxY = WorldManager::GetInstance()->GetScale();

	//SetIndex
	mStart = Vec(x, y);
	mEnd = Vec(endX, endY);
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

bool AStar::OnA_Star(std::pair<int,int> idx)
{
	if (mbRun)
		return false;

	/*if ()
		return false;*/

	ClearNode();

	mMaxX = WorldManager::GetInstance()->GetTileDataWorldSize();
	mMaxY = WorldManager::GetInstance()->GetTileDataWorldSize();

	//SetIndex
	Transform* tr = GetOwner()->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();

	auto startIdx = WorldManager::GetInstance()->GetTileIndex(Vector2(pos.x, pos.y));
	mStart = Vec(startIdx.first, startIdx.second);

	mEnd = Vec(idx.first, idx.second);
	mbRun = true;

	// SetNode
	mCurNode = Node{};

	mCurNode.Pos = Vec(mStart);
	mCurNode.Id = (mCurNode.Pos.y * mMaxY) + (mCurNode.Pos.x % mMaxX);
	mCurNode.Cost = 0;
	mCurNode.Heuristick = GetHeuristick(mCurNode.Pos.x, mCurNode.Pos.y);
	mCurNode.Distance = mCurNode.GetDistance();

	auto TileIdx = Input::GetInstance()->GetIsoMetricIDX(Vector2(pos.x, pos.y));
	mCurNode.Tile = ObjectManager::GetInstance()->GetTile(TileIdx.first, TileIdx.second);

	if (mCurNode.Tile == nullptr)
		return false;

	mCloseList.emplace(make_pair(mCurNode.Id, mCurNode));

	return false;
}

void AStar::Compare(Node duplication)
{
	UINT g = mCurNode.Cost;

	int x = abs(mCurNode.Pos.x - duplication.Pos.x);
	int y = abs(mCurNode.Pos.y - duplication.Pos.y);

	if (x + y > 1)
		g += 14;
	else
		g += 10;

	if (duplication.Cost > g)
	{
		duplication.ParentIndex = mCurNode.Pos;

		duplication.Cost = g;
		duplication.Distance = duplication.Cost + duplication.Heuristick;
	}

	unordered_map<UINT, Node>::iterator iter;
	iter = mOpenList.find(duplication.Id);
	iter->second = duplication;

	vector<Node> NodeTemp;
	while (!mDistanceList.empty())
	{
		Node temp = mDistanceList.top();
		mDistanceList.pop();

		if (duplication.Id == temp.Id)
			break;

		NodeTemp.emplace_back(temp);
	}

	mDistanceList.emplace(duplication);
	for (int i = 0; i < NodeTemp.size(); ++i)
	{
		mDistanceList.emplace(NodeTemp[i]);
	}

	NodeTemp.clear();
}

AStar::Node AStar::DistanceList()
{
	Node node = mDistanceList.top();
	mDistanceList.pop();
	RemoveOpenList(node);

	return node;
}

void AStar::Result()
{
	while (!mResult.empty())
	{
		mResult.pop();
	}

	unordered_map<UINT, Node>::iterator iter;
	UINT count = 0;

	Node node = mCurNode;
	while (1)
	{
		if ((mCurNode.Pos == mStart))
		{
			WorldManager::GetInstance()->SetEndIndex(node.Pos.x, node.Pos.y);
			// 맨 처음 시작 노드
			mResult.push(mCurNode);

			if (mResult.size() > 0)
				mbNodeEmpty = false;

			mbRun = false;
			//GetCarveTileAStar();
			
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

void AStar::CarveTileAStar()
{
	if (mbNodeEmpty)
		return;


}

void AStar::ClearNode()
{
	mOpenList.clear();
	mCloseList.clear();

	while (!mResult.empty()) { mResult.pop(); }
	while (!mDistanceList.empty()) { mDistanceList.pop(); }
}

AStar::Node* AStar::GetNextNode()
{
	if (mResult.empty())
	{
		mbNodeEmpty = true;
		return nullptr;
	}
	else
	{
		Node* node = &(mResult.top());
		mResult.pop();

		if (mResult.empty())
			mbNodeEmpty = true;

		return node;
	}
	return nullptr;
}