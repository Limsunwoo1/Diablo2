#include "CCollider2D.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CRenderer.h"


Collider2D::Collider2D()
	: Component(eComponentType::Collider)
	, mType(eColliderType::None)
	, mTransform(nullptr)
	, mSize(Vector2::One)
	, mCenter(Vector2::Zero)
	, mbTrigger(false)
{
	
}

Collider2D::~Collider2D()
{

}

void Collider2D::Initalize()
{
	mTransform = GetOwner()->GetComponent<Transform>();
}

void Collider2D::Update()
{

}

void Collider2D::FixedUpdate()
{
	Vector3 scale = mTransform->GetScale();
	scale *= Vector3(mSize.x, mSize.y, 1.0f);

	Vector3 rotation = mTransform->GetRotation();

	Vector3 colliderPos = mTransform->GetPosition();
	colliderPos += Vector3(mCenter.x, mCenter.y, 0.0f);

	Matrix scaleMatrix = Matrix::CreateScale(scale);
	Matrix rotationMatrix;
	rotationMatrix = Matrix::CreateRotationX(rotation.x);
	rotationMatrix *= Matrix::CreateRotationY(rotation.y);
	rotationMatrix *= Matrix::CreateRotationZ(rotation.z);

	Matrix positionMatrix;
	positionMatrix.Translation(Vector3(colliderPos.x, colliderPos.y, colliderPos.z));

	Matrix worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

	DebugMesh meshAttricbute = {};
	meshAttricbute.position = Vector3(colliderPos.x, colliderPos.y, colliderPos.z);
	meshAttricbute.radius = 1.0f;
	meshAttricbute.rotation = rotation;
	meshAttricbute.scale = scale;
	meshAttricbute.type = mType;

	Renderer::debugMeshes.push_back(meshAttricbute);
}

void Collider2D::Render()
{

}
