#include "CAStar.h"
#include "CWorldManager.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CPlayerScript.h"
#include "CTileObject.h"
#include "CWorldManager.h"
#include "CInput.h"
#include "CObjectManager.h"

using namespace Math;
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
	{
		ClearNode();
		return;
	}

	int count = 0;
	while (1)
	{
		if (mbRun == false)
			return;

		if (count >= 100)
		{
			mbRun = false;
			break;
		}

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
		count++;
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

	int tem = abs(ix + iy);
	int weight = 1;
	if (tem == 0)
		weight = 10;
	else
		weight = 14;

	UINT H = (ix + iy) * 10;
	return H;
}

UINT AStar::GetHeuristick(int x, int y, Vec End)
{
	int ix = abs(x - End.x);
	int iy = abs(y - End.y);

	int tem = abs(ix + iy);
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

UINT AStar::GetHeuristick2(int x, int y, Vec End)
{
	int ix = abs(x - End.x);
	int iy = abs(y - End.y);

	int weight = mCloseList.size();
	// 상황에 따른 가중치 알고리즘 추가하면
	// 조금 더 정교한 탐색이 가능해진다
	/*if (tem == 0)
		weight = 14;
	else
		weight = 10;*/

	return (int)(1 * (std::sqrt(std::pow(x - End.x, 2) + std::pow(y - End.y, 2))));
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

UINT AStar::GetID(int x, int y, int maxX, int maxY)
{
	UINT id = (y * maxY) + (x % maxX);
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

	if (y < 0 || y >= mMaxY)
		return;

	if (x < 0 || x >= mMaxX)
		return;

	node.Tile = Tiles[y][x];
	if (node.Tile == nullptr)
		return;
	//CurNode

	// 맵 범위 밖 타일인지확인
	if (y < 0 || y >= Tiles.size())
		return;

	if (x < 0 || x >= Tiles[y].size())
		return;


	// 해당타일이 장애물이 아닌지 확인

	if (node.Tile->GetPass() == false)
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
		{
			if (mCurNode.Tile->GetArr()[3] != 0 || node.Tile->GetArr()[0] != 0) check = true;
		}
		else if (diffX < 0 && diffY < 0)
		{
			if (mCurNode.Tile->GetArr()[1] != 0 || node.Tile->GetArr()[2] != 0) check = true;
		}
		else if (diffX > 0 && diffY < 0)
		{
			if (mCurNode.Tile->GetArr()[0] != 0 || node.Tile->GetArr()[3] != 0) check = true;
		}
		else if (diffX > 0 && diffY > 0)
		{
			if (mCurNode.Tile->GetArr()[2] != 0 || node.Tile->GetArr()[1] != 0) check = true;
		}

		if (check)
			return;
	}

	int count = abs(mCurNode.Pos.x - node.Pos.x) + abs(mCurNode.Pos.y - node.Pos.y);
	if (count > 1)
		node.Cost = 10 + mCurNode.Cost;
	else
		node.Cost = 14 + mCurNode.Cost;

	node.Heuristick = GetHeuristick(node.Pos.x, node.Pos.y);
	node.Distance = node.GetDistance();

	node.ParentIndex = mCurNode.Pos;

	mOpenList.insert(make_pair(node.Id, node));
	mDistanceList.emplace(node);
}

void AStar::CarveAddOpenList(int x, int y, Vec End, bool diagonal)
{
	Node node = {};

	// 포즈가 현재 화면상인덱스 임
	// 시작 노드의 idx 를 변환해줘야함
	node.Pos.x = x;
	node.Pos.y = y;

	node.Id = GetID(node.Pos.x, node.Pos.y, 20, 20);

	if (y < 0 || y >= mTilesCarveData.size())
		return;

	if (x < 0 || x >= mTilesCarveData[y].size())
		return;

	node.Tile = mTilesCarveData[y][x].tile;

	if (mTilesCarveData[y][x].ArrValue == 1)
		return;

	//if (mFindResult.find((UINT)node.Tile) == mFindResult.end())
	//	return;

		// 맵 범위 밖 타일인지확인

		// 닫힌 목록에 있는 노드인지 탐색
	unordered_map<UINT, Node>::iterator iter;
	iter = mCloseList.find(node.Id);

	if (iter != mCloseList.end())
		return;

	// 오픈노드에 있는 노드인지 탐색
	iter = mOpenList.find(node.Id);
	if (iter != mOpenList.end())
	{
		// G 값 비교후 들어있는 
		// 타일 데이터 수정 G, F
		CarveCompare(iter->second, true);
		return;
	}

	if (diagonal)
	{
		if (mTilesCarveData[mCarveCurNode.Pos.x][node.Pos.y].ArrValue == 1)
			return;

		if (mTilesCarveData[node.Pos.y][mCarveCurNode.Pos.x].ArrValue == 1)
			return;
	}

	int count = abs(mCarveCurNode.Pos.x - node.Pos.x) + abs(mCarveCurNode.Pos.y - node.Pos.y);
	if (count > 1)
		node.Cost = 14 + mCarveCurNode.Cost;
	else
		node.Cost = 10 + mCarveCurNode.Cost;

	node.Heuristick = GetHeuristick(node.Pos.x, node.Pos.y, End);
	node.Distance = node.GetDistance();

	node.ParentIndex = mCarveCurNode.Pos;

	mOpenList.insert(make_pair(node.Id, node));
	mDistanceList.emplace(node);
}

void AStar::AddCloseList()
{
	Node node = DistanceList();
	if (node.Tile == nullptr)
	{
		mbRun = false;
		ClearNode();
		return;
	}

	mCurNode = node;
	mCloseList.emplace(std::make_pair(mCurNode.Id, mCurNode));
}

void AStar::CarveAddCloseList()
{
	Node node = DistanceList();

	if (node.Tile == nullptr)
	{
		mbRun = false;
		ClearNode();
	}

	mCarveCurNode = node;
	mCloseList.emplace(std::make_pair(mCarveCurNode.Id, mCarveCurNode));
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

bool AStar::OnA_Star(std::pair<int, int> idx, Vector2 endPos)
{

	/*if ()
		return false;*/

	ClearNode();

	bool RunCheck = false;
	auto RuntileIdx = Input::GetInstance()->GetIsoMetricIDX(endPos);
	TileObject* tile = ObjectManager::GetInstance()->GetTile(RuntileIdx.first, RuntileIdx.second);
	int arrNum = -1;

	if (tile->PickTile(endPos, eTilePickType::Tile0)) { arrNum = (UINT)eTilePickType::Tile0; }
	else if (tile->PickTile(endPos, eTilePickType::Tile1)) { arrNum = (UINT)eTilePickType::Tile1; }
	else if (tile->PickTile(endPos, eTilePickType::Tile2)) { arrNum = (UINT)eTilePickType::Tile2; }
	else if (tile->PickTile(endPos, eTilePickType::Tile3)) { arrNum = (UINT)eTilePickType::Tile3; }

	if (arrNum < 0)
		return false;

	if (tile->GetArr()[arrNum] == 1)
		return false;

	Tiles = WorldManager::GetInstance()->DropWordTileData();
	mTilesCarveData = WorldManager::GetInstance()->DropWolrdTileCarveData();

	mMaxX = WorldManager::GetInstance()->GetTileDataWorldSize();
	mMaxY = WorldManager::GetInstance()->GetTileDataWorldSize();

	//SetIndex
	Transform* tr = GetOwner()->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();
	//
	pos.y -= tr->GetSize().y * 0.25f;

	auto startIdx = WorldManager::GetInstance()->GetTileIndex(Vector2(pos.x, pos.y));
	if (startIdx.first < 0 || startIdx.second < 0)
		return false;

	auto EndIdx = WorldManager::GetInstance()->GetTileIndex(Vector2(endPos.x, endPos.y));
	if (EndIdx.first < 0 || EndIdx.second < 0)
		return false;

	//mStart = Vec(startIdx.first, startIdx.second);
	mStart = Vec(startIdx.first, startIdx.second);

	mEnd = Vec(EndIdx.first, EndIdx.second);
	mEndPos = endPos;
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

	mStartNode = mCurNode;
	mCloseList.emplace(make_pair(mCurNode.Id, mCurNode));

	return true;
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

void AStar::CarveCompare(Node duplication, bool carve)
{
	UINT g = mCarveCurNode.Cost;

	int x = abs(mCarveCurNode.Pos.x - duplication.Pos.x);
	int y = abs(mCarveCurNode.Pos.y - duplication.Pos.y);

	if (x + y > 1)
		g += 14;
	else
		g += 10;

	if (duplication.Cost > g)
	{
		duplication.ParentIndex = mCarveCurNode.Pos;

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
	if (mDistanceList.empty())
		return Node();

	Node node = mDistanceList.top();
	mDistanceList.pop();
	RemoveOpenList(node);

	return node;
}

void AStar::Result()
{
	mResult.clear();

	unordered_map<UINT, Node>::iterator iter;
	UINT count = 0;

	Node node = mCurNode;
	while (1)
	{
		if ((mCurNode.Pos == mStart))
		{
			// 맨 처음 시작 노드
			mResult.emplace_back(mStartNode);

			CarveTileAStar();
			return;
		}

		if (count >= 100)
		{
			mbRun = false;
			break;
		}

		count++;

		mResult.emplace_back(mCurNode);

		int id = (mCurNode.ParentIndex.y * mMaxY) + (mCurNode.ParentIndex.x % mMaxX);
		iter = mCloseList.find(id);
		if (iter == mCloseList.end())
			continue;

		mCurNode = iter->second;
	}
}

void AStar::CarveTileAStar()
{
	if (!PosData.empty())
	{
		mbRun = false;
		return;
	}


	ResultIter::iterator iter = mResult.begin();

	if (iter == mResult.end())
		return;

	Node Start = {};
	Node End = {};

	if (mResult.size() == 1)
	{
		Start = mResult[0];
		End = mResult[0];
	}
	else
	{
		auto beginIter = mResult.begin();

		auto Enditer = mResult.end();
		--Enditer;

		Start = *Enditer;
		End = *beginIter;
	}

	for (Node& node : mResult)
	{
		if (node.Tile == nullptr)
			continue;

		mFindResult.insert(std::make_pair((UINT)node.Tile, node));
	}

	vector<pair<pair<int, int>, int>> positionData;

	int count = 0;

	Transform* tr = GetOwner()->GetComponent<Transform>();

	Vector3 pos = tr->GetPosition();
	pos.y -= tr->GetSize().y * 0.25f;

	Vector2 StartPos = Vector2(pos.x, pos.y);
	Vector2 EndPos = mEndPos;

	int startIndex = -1;
	if (Start.Tile->PickTile(StartPos, eTilePickType::Tile0)) { startIndex = (UINT)eTilePickType::Tile0; }
	else if (Start.Tile->PickTile(StartPos, eTilePickType::Tile1)) { startIndex = (UINT)eTilePickType::Tile1; }
	else if (Start.Tile->PickTile(StartPos, eTilePickType::Tile2)) { startIndex = (UINT)eTilePickType::Tile2; }
	else if (Start.Tile->PickTile(StartPos, eTilePickType::Tile3)) { startIndex = (UINT)eTilePickType::Tile3; }

	int endIndex = -1;
	if (End.Tile->PickTile(EndPos, eTilePickType::Tile0)) { endIndex = (UINT)eTilePickType::Tile0; }
	else if (End.Tile->PickTile(EndPos, eTilePickType::Tile1)) { endIndex = (UINT)eTilePickType::Tile1; }
	else if (End.Tile->PickTile(EndPos, eTilePickType::Tile2)) { endIndex = (UINT)eTilePickType::Tile2; }
	else if (End.Tile->PickTile(EndPos, eTilePickType::Tile3)) { endIndex = (UINT)eTilePickType::Tile3; }

	if (startIndex < 0 || endIndex < 0)
		return;

	Start.Pos *= 2;
	if (startIndex == 0) { Start.Pos.y = (Start.Pos.y + 1); }
	else if (startIndex == 1) { Start.Pos = Vec((Start.Pos.x + 1), (Start.Pos.y + 1)); }
	else if (startIndex == 2) {}
	else if (startIndex == 3) { Start.Pos.x = (Start.Pos.x + 1); }

	End.Pos *= 2;
	if (endIndex == 0) { End.Pos.y = (End.Pos.y + 1); }
	else if (endIndex == 1) { End.Pos = Vec((End.Pos.x + 1), (End.Pos.y + 1)); }
	else if (endIndex == 2) {}
	else if (endIndex == 3) { End.Pos.x = (End.Pos.x + 1); }

	/*Start.Pos *= 2;
	if (startIndex == 0) {}
	else if (startIndex == 1) { Start.Pos.x = (Start.Pos.x + 1); }
	else if (startIndex == 2) { Start.Pos.y = (Start.Pos.y + 1); }
	else if (startIndex == 3) { Start.Pos = Vec((Start.Pos.x + 1), (Start.Pos.y + 1)); }

	End.Pos *= 2;
	if (endIndex == 0) {}
	else if (endIndex == 1) { End.Pos.x = (End.Pos.x + 1); }
	else if (endIndex == 2) { End.Pos.y = (End.Pos.y + 1); }
	else if (endIndex == 3) { End.Pos = Vec((End.Pos.x + 1), (End.Pos.y + 1)); }*/

	/*std::cout << Start.Pos.x << "    Start X    " << Start.Pos.y << "        Start Y   <<  " << endl;
	std::cout << End.Pos.x << "    End X    " << End.Pos.y << "        End Y   <<  " << endl;*/

	Node CurNode = {};
	CurNode.Pos = Start.Pos;
	CurNode.Tile = Start.Tile;
	// 20 == TileCarveData.size()
	CurNode.Id = GetID(CurNode.Pos.x, CurNode.Pos.y, 20, 20);
	mCarveCurNode = CurNode;
	mCarveStartNode = CurNode;

	Node EndNode = {};
	EndNode.Pos = End.Pos;
	EndNode.Tile = End.Tile;
	// 20 == TileCarveData.size()
	EndNode.Id = GetID(EndNode.Pos.x, EndNode.Pos.y, 20, 20);

	mCarveEndNode = EndNode;

	mCloseList.clear();
	mOpenList.clear();

	while (!mDistanceList.empty())
	{
		mDistanceList.pop();
	}

	mCloseList.insert(std::make_pair(mCarveCurNode.Id, mCarveCurNode));

	while (1)
	{
		if (mbRun == false)
			return;

		if (count >= 100)
		{
			mbRun = false;
			break;
		}

		if (mCarveCurNode.Pos == mCarveEndNode.Pos)
		{
			SaveCarvePosData();
			return;
		}

		// ↗ ↘ ↙ ↖
		int arr1X[4] = { 1, 1, -1, -1 };
		int arr1Y[4] = { -1, 1, 1, -1 };

		for (int i = 0; i < 4; ++i) { CarveAddOpenList(mCarveCurNode.Pos.x + arr1X[i], mCarveCurNode.Pos.y + arr1Y[i], mCarveEndNode.Pos); }

		// → ↓ ← ↑
		int arr2X[4] = { 1, 0, -1, 0 };
		int arr2Y[4] = { 0, 1, 0, -1 };

		for (int i = 0; i < 4; ++i) { CarveAddOpenList(mCarveCurNode.Pos.x + arr2X[i], mCarveCurNode.Pos.y + arr2Y[i], mCarveEndNode.Pos, false); }

		CarveAddCloseList();
		count++;
	}
}

void AStar::SaveCarvePosData()
{
	mResult.clear();
	mFindResult.clear();

	while (!PosData.empty())
	{
		PosData.pop();
	}

	unordered_map<UINT, Node>::iterator iter;
	UINT count = 0;

	std::stack<Vector2> pathStack;
	std::vector<Node> temp;
	Node node = mCarveCurNode;

	while (1)
	{
		if ((mCarveCurNode.Pos == mCarveStartNode.Pos))
		{
			vector<pair<int, int>> directionVec;

			if (temp.size() == 0)
				return;

			for (int i = 0; i < temp.size() - 1; ++i)
			{
				directionVec.push_back(pair(temp[i].Pos.x - temp[i + 1].Pos.x
					, temp[i].Pos.y - temp[i + 1].Pos.y));
			}

			if (directionVec.size() == 1)
				return;

			for (int i = 0; i < directionVec.size() - 1; ++i)
			{
				if (directionVec[i] != directionVec[i + 1])
				{
					Vector2 Pos = GetCarvePos(temp[i]);
					pathStack.push(Pos);
				}
			}

			if (pathStack.empty())
				pathStack.push(mEndPos);

			PosData = pathStack;
			Script* script = GetOwner()->GetScript<Script>();
			script->SetPosData(PosData);

			return;
		}

		if (count >= 100)
		{
			mbRun = false;
			break;
		}

		Vector2 pos = Vector2(-1.f, -1.f);
		pos = GetCarvePos(mCarveCurNode);
		//test.push(mCarveCurNode);
		temp.push_back(mCarveCurNode);

		if (pos.x < 0 || pos.y < 0)
			return;

		PosData.emplace(pos);

		int id = (mCarveCurNode.ParentIndex.y * 20) + (mCarveCurNode.ParentIndex.x % 20);
		iter = mCloseList.find(id);
		if (iter == mCloseList.end())
			continue;

		mCarveCurNode = iter->second;
		count++;
	}
}


Vector2 AStar::GetCarvePos(Node curNode)
{
	/* { End.Pos.y = (End.Pos.y + 1); }
	{ End.Pos = Vec((End.Pos.x + 1), (End.Pos.y + 1)); }
	{ End.Pos.x = (End.Pos.x + 1); }
	{  }*/

	Vector2 pos = Vector2(-1.f, -1.f);

	int diffX = curNode.Pos.x % 2;
	int diffY = curNode.Pos.y % 2;

	eTilePickType type = eTilePickType::TileALL;

	if (diffX <= 0 && diffY <= 0) { type = eTilePickType::Tile2; }
	else if (diffX > 0 && diffY <= 0) { type = eTilePickType::Tile3; }
	else if (diffX <= 0 && diffY > 0) { type = eTilePickType::Tile0; }
	else /*diffX > 0 && diffY > 0*/ { type = eTilePickType::Tile1; }

	if (curNode.Tile != nullptr)
		pos = mTilesCarveData[curNode.Pos.y][curNode.Pos.x].tile->TileCarvePos(type);

	return pos;
}

void AStar::ClearNode()
{
	mOpenList.clear();
	mCloseList.clear();
	mResult.clear();
	mFindResult.clear();

	while (!PosData.empty()) { PosData.pop(); }
	while (!mDistanceList.empty()) { mDistanceList.pop(); }
}

AStar::Node* AStar::GetNextNode()
{
	/*if (mResult.empty())
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
	}*/
	return nullptr;
}

void AStar::PopNode()
{
	if (!PosData.empty())
	{
		PosData.pop();
	}
}
