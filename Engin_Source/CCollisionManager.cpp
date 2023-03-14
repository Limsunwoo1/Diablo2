#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CCollider2D.h"
#include "CTransform.h"

CollisionManager::CollisionManager()
{

}

CollisionManager::~CollisionManager()
{

}

void CollisionManager::Initalize()
{

}

void CollisionManager::Update()
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

void CollisionManager::FixedUpdate()
{
}

void CollisionManager::Render()
{
}

void CollisionManager::CollisionlayerCheck(eLayerType left, eLayerType right, bool enable)
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

void CollisionManager::LayerCollision(Scene* scene, eLayerType left, eLayerType right)
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
			if (left == right)
				continue;

			ColliderCollision(left->GetComponent<Collider2D>(), right->GetComponent<Collider2D>());
		}
	}
}

void CollisionManager::ColliderCollision(Collider2D* left, Collider2D* right)
{
	// 두 충돌체의 레이어로 구성된 ID 확인
	ColliderID colliderID;
	colliderID.left = (UINT)left->GetID();
	colliderID.right = (UINT)right->GetID();

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

bool CollisionManager::Intersect(Collider2D* left, Collider2D* right)
{
	eColliderType leftType = left->GetType();
	eColliderType rightType = right->GetType();

	if (leftType == eColliderType::Rect && rightType == eColliderType::Rect)
		return Rect_VS_Rect(left, right);
	else if (leftType == eColliderType::Circle && rightType == eColliderType::Circle)
		return Circle_VS_Circle(left, right);

	return false;
}

bool CollisionManager::Rect_VS_Rect(Collider2D* left, Collider2D* right)
{
	// Rect vs Rect

	// 0 ----- 1
	// l       l
	// 3 ----- 2
	static const Vector3 arrLocalPos[4] =
	{
		Vector3{-0.5f, 0.5f, 0.0f}
		,Vector3{0.5f, 0.5f, 0.0f}
		,Vector3{0.5f, -0.5f, 0.0f}
		,Vector3{-0.5f, -0.5f, 0.0f}
	};

	Transform* leftTransform = left->GetOwner()->GetComponent<Transform>();
	Transform* rightTransform = right->GetOwner()->GetComponent<Transform>();

	Matrix leftMatrix = leftTransform->GetWorldMatrix();
	Matrix rightMatrix = rightTransform->GetWorldMatrix();

	// 분리축 벡터 4 개 구하기
	Vector3 Axis[4] = {};
	Axis[0] = Vector3::Transform(arrLocalPos[1], leftMatrix);
	Axis[1] = Vector3::Transform(arrLocalPos[3], leftMatrix);
	Axis[2] = Vector3::Transform(arrLocalPos[1], rightMatrix);
	Axis[3] = Vector3::Transform(arrLocalPos[3], rightMatrix);

	Axis[0] -= Vector3::Transform(arrLocalPos[0], leftMatrix);
	Axis[1] -= Vector3::Transform(arrLocalPos[0], leftMatrix);
	Axis[2] -= Vector3::Transform(arrLocalPos[0], rightMatrix);
	Axis[3] -= Vector3::Transform(arrLocalPos[0], rightMatrix);

	Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.0f);
	Axis[0] = Axis[0] * leftScale;
	Axis[1] = Axis[1] * leftScale;

	Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.0f);
	Axis[2] = Axis[2] * leftScale;
	Axis[3] = Axis[3] * leftScale;


	for (int i = 0; i < 4; ++i)
	{
		Axis[i].z = 0.0f; // z 값 초기화
	}

	Vector3 vc = leftTransform->GetPosition() - rightTransform->GetPosition();
	vc.z = 0.0f;

	Vector3 centerDir = vc;
	for (int i = 0; i < 4; ++i)
	{
		Vector3 vA = Axis[i];
		//vA.Normalize();

		float projDist = 0.0f;
		for (int j = 0; j < 4; ++j)
		{
			projDist += fabsf(Axis[j].Dot(vA) * 0.5f);
		}

		if (projDist < fabsf(centerDir.Dot(vA)))
			return false;
	}
	return true;
}

bool CollisionManager::Circle_VS_Circle(Collider2D* left, Collider2D* right)
{
	Transform* leftTransform = left->GetOwner()->GetComponent<Transform>();
	Transform* rightTransform = right->GetOwner()->GetComponent<Transform>();

	Vector3 leftPos = left->GetPosition();
	Vector3 rightPos = right->GetPosition();

	Vector2 leftScale = left->GetSize();
	Vector2 rightScale = right->GetSize();

	Vector3 vect = leftPos - rightPos;
	vect.z = 0.0f;

	float dist = vect.Length();

	float len = sqrt((vect.x * vect.x) + (vect.y * vect.y));

	float sum = leftScale.x / 2.0f + rightScale.x / 2.0f;

	if (dist <= sum)
		return true;
	else
		return false;

	
}
