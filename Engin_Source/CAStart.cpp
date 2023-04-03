#include "CAStart.h"
#include "CWorldManager.h"

AStar::AStar()
	: Component(eComponentType::AStart)
	, mMaxX(0)
	, mMaxY(0)
	, mStart(-1, -1)
	, mEnd(-1, -1)
	, mbRun(false)
	, mCurNode(nullptr)
{
}

AStar::~AStar()
{
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
		if (WorldManager::GetInstance()->TileNum(mCurNode->V.X, mCurNode->V.Y) == 2)
		{
			Result();
			return;
		}

		// ↗ ↘ ↙ ↖
		int arr1X[4] = { 1, 1, -1, -1 };
		int arr1Y[4] = { -1, 1, 1, -1 };

		for (int i = 0; i < 4; ++i) { PushOpenList(mCurNode->V.X + arr1X[i], mCurNode->V.Y + arr1Y[i]); }

		// → ↓ ← ↑
		int arr2X[4] = { 1, 0, -1, 0 };
		int arr2Y[4] = { 0, 1, 0, -1 };

		for (int i = 0; i < 4; ++i) { PushOpenList(mCurNode->V.X + arr2X[i], mCurNode->V.Y + arr2Y[i], false); }

		Node* node = PushCloseList();
		DeleteOpenList(node);
	}
}

void AStar::Render()
{
}

UINT AStar::H(int x, int y)
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



	int ix = abs(x - mEnd.X);
	int iy = abs(y - mEnd.Y);

	int tem = abs(ix - iy);
	int weight = 1;
	if (tem == 0)
		weight = 14;
	else
		weight = 10;

	UINT H = (ix + iy) * 10;
	return H + weight;

}

UINT AStar::G(int x, int y, int InG)
{
	int ix = abs(x - mEnd.X);
	int iy = abs(y - mEnd.Y);

	int tem = abs(ix + iy);

	int outG = InG;
	if (tem > 1)
		outG += 14;
	else
		outG += 10;

	return outG;
}

//long long int AStart::F(int g, int h)
//{
//    return g + h;
//}

UINT AStar::ID(int x, int y)
{
	UINT id = (y * mMaxY) + (x % mMaxX);
	return id;
}

void AStar::PushOpenList(int x, int y, bool diagonal)
{
	Node node = {};

	node.V.X = x;
	node.V.Y = y;

	// 맵 범위 밖 타일인지확인
	if (node.V.X  < 0 || node.V.X  > mMaxX - 1)
		return;

	if (node.V.Y < 0 || node.V.Y > mMaxY - 1)
		return;

	// 해당타일이 장애물이 아닌지 확인
	if (WorldManager::GetInstance()->TileNum(node.V.X, node.V.Y) == 3)
		return;

	// 대각 선이동의 경우 해당타일의
	// 직각이 되는 타일이 장애물이면 리턴
	if (diagonal)
	{
		if (WorldManager::GetInstance()->TileNum(mCurNode->V.X, node.V.Y) == 3)
			return;

		if (WorldManager::GetInstance()->TileNum(node.V.X, mCurNode->V.Y) == 3)
			return;
	}


	// 닫힌 목록에 있는 노드인지 탐색
	unordered_map<UINT, Node*>::iterator iter;
	iter = mCloseList.find(node.Id);

	if (iter != mCloseList.end())
		return;

	// 오픈노드에 있는 노드인지 탐색
	iter = mOpenList.find(node.Id);
	if (iter != mOpenList.end())
	{
		// G 값 비교후 들어있는 
		// 타일 데이터 수정 G, H
		Test(iter->second);
		return;
	}

	int count = abs(mCurNode->V.X - node.V.X) + abs(mCurNode->V.Y - node.V.Y);
	if (count > 1)
		node.GValue = 14 + mCurNode->GValue;
	else
		node.GValue = 10 + mCurNode->GValue;

	node.HValue = H(node.V.X, node.V.Y);
	node.FValue = node.F();

	node.Id = ID(node.V.X, node.V.Y);

	node.PartenNode = mCurNode;

	Node* pushNode = new Node(node);

	mOpenList.insert(make_pair(pushNode->Id, pushNode));
	mOpenlistF.push( pushNode);


}

AStar::Node* AStar::PushCloseList()
{
	Node* node = LowFValue();
	if (node == nullptr)
		cout << " 길찾기 실패" << endl;

	mCurNode = node;
	mCloseList.insert(make_pair(mCurNode->Id, mCurNode));
	return node;
}

void AStar::DeleteOpenList(Node* node)
{
	unordered_map<UINT, Node*>::iterator iter;
	iter = mOpenList.find(node->Id);
	if (iter != mOpenList.end())
		mOpenList.erase(iter);
}

void AStar::OnA_Star(Node& node, Vec start, Vec end, bool run)
{
	if (mbRun)
		return;

	ClearNode();

	mMaxX = WorldManager::GetInstance()->GetScale();
	mMaxY = WorldManager::GetInstance()->GetScale();

	mCurNode = new Node(node);

	mStart = start;
	mEnd = end;

	mbRun = run;

	mCloseList.insert(make_pair(mCurNode->Id, mCurNode));
}

void AStar::Test(Node* duplication)
{
	UINT g = mCurNode->GValue;

	int x = abs(mCurNode->V.X - duplication->V.X);
	int y = abs(mCurNode->V.Y - duplication->V.Y);

	if (x + y > 1)
		g = mCurNode->GValue += 14;
	else
		g = mCurNode->GValue += 10;

	if (duplication->GValue > g)
	{
		duplication->PartenNode = mCurNode;

		duplication->GValue = g;
		duplication->FValue = duplication->GValue + duplication->HValue;
	}
}

AStar::Node* AStar::LowFValue()
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


	Node* node = mOpenlistF.top();
	mOpenlistF.pop();

	return node;

}

stack<AStar::Node*>& AStar::Result()
{
	while (!mResult.empty())
	{
		Node* node = mResult.top();
		mResult.pop();
	}

	while (1)
	{
		if ((mCurNode->V.X == mStart.X &&
			mCurNode->V.Y == mStart.Y) ||
			mCurNode->PartenNode == nullptr)
		{
			mbRun = false;
			return mResult;
		}

		mResult.push(mCurNode);
		mCurNode = mCurNode->PartenNode;
	}
}

void AStar::Clear()
{
	mMaxX = WorldManager::GetInstance()->GetScale();
	mMaxY = WorldManager::GetInstance()->GetScale();

	mStart = Vec{ -1, -1 };
	mEnd = Vec{ -1, -1 };

	ClearNode();
}

void AStar::ClearNode()
{
	unordered_map<UINT, Node*>::iterator iter;
	for (iter = mOpenList.begin();
		iter != mOpenList.end();
		iter++)
	{
		delete iter->second;
		iter->second = nullptr;
	}

	for (iter = mCloseList.begin();
		iter != mCloseList.end();
		iter++)
	{
		delete iter->second;
		iter->second = nullptr;
	}

	mOpenList.clear();
	mCloseList.clear();
}

bool AStar::FSort(std::pair<UINT, Node*>& a, std::pair<UINT, Node*>& b)
{
	return a.second->F() < b.second->F();
}
