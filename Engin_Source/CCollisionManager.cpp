#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CCollider2D.h"

CCollisionManager::CCollisionManager()
{

}

CCollisionManager::~CCollisionManager()
{

}

void CCollisionManager::Initalize()
{

}

void CCollisionManager::Update()
{
	Scene* scene = SceneManager::GetInstance()->GetActiveScene();
	for (UINT row = 0; row < (UINT)eLayerType::End; ++row)
	{
		for (UINT column = 0; column < (UINT)eLayerType::End; ++column)
		{
			if (mLayerCollisionMatrix[row][column] == true)
			{
				LayerCollision(scene, (eLayerType)row, (eLayerType)column);
			}

		}
	}
}

void CCollisionManager::FixedUpdate()
{
}

void CCollisionManager::Render()
{
}

void CCollisionManager::CollisionlayerCheck(eLayerType left, eLayerType right, bool enable)
{
	int row = 0;
	int col = 0;

	if ((UINT)left <= (UINT)right)
	{
		row = (UINT)left;
		col = (UINT)right;
	}
	else
	{
		row = (UINT)right;
		col = (UINT)left;
	}

	mLayerCollisionMatrix[row][col] = enable;
}

void CCollisionManager::LayerCollision(Scene* scene, eLayerType left, eLayerType right)
{
	const std::vector<GameObject*>& lefts = scene->GetGameObject(left);
	const std::vector<GameObject*>& rights = scene->GetGameObject(right);

	for (GameObject* left : lefts)
	{
		if (left->GetState() != GameObject::active)
			continue;
		if (left->GetComponent<Collider2D>() == nullptr)
			continue;

		for (GameObject* right : rights)
		{
			if (right->GetState() != GameObject::active)
				continue;
			if (right->GetComponent<Collider2D>() == nullptr)
				continue;
			if (left = right)
				continue;

			ColliderCollision(left->GetComponent<Collider2D>(), right->GetComponent<Collider2D>());
		}
	}
}

void CCollisionManager::ColliderCollision(Collider2D* left, Collider2D* right)
{
	// 두 충돌체의 레이어로 구성된 ID 확인
	ColliderID colliderID;
	colliderID.left = (UINT)left;
	colliderID.right = (UINT)right;

	// 이전 충돌정보를 검색한다
	// 만약 충돌정보가 없는 상태라면
	// 충돌 정보를 생성해준다
	std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.id);
	if (iter == mCollisionMap.end())
	{
		mCollisionMap.insert(make_pair(colliderID.id, false));
		iter = mCollisionMap.find(colliderID.id);
	}

	// 충돌체크를 해준다
	if (Intersect(left, right)) // 충돌을 한 상태
	{
		// 최초 충돌중 Enter
		if (iter->second == false)
		{
			if (left->IsTrigger())
				left->OnTriggerEnter(right);
			else
				left->OnCollisionEnter(right);

			if (right->IsTrigger())
				right->OnTriggerEnter(left);
			else
				right->OnCollisionEnter(left);

			iter->second = true;
		}
		else // 충돌 중
		{
			if (left->IsTrigger())
				left->OnTriggerStay(right);
			else
				left->OnCollisionStay(right);

			if (right->IsTrigger())
				right->OnTriggerStay(left);
			else
				right->OnCollisionStay(left);
		}
	}
	else // 충돌하지 않은 상태
	{
		// 충돌 중인 상태 Exit
		if (iter->second == true)
		{
			if (left->IsTrigger())
				left->OnTriggerExit(right);
			else
				left->OnCollisionExit(right);

			if (right->IsTrigger())
				right->OnTriggerExit(left);
			else
				right->OnCollisionExit(left);

			iter->second = false;
		}
	}
}

bool CCollisionManager::Intersect(Collider2D* left, Collider2D* right)
{
	return true;
}
