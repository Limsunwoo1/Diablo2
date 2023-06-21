#include "CCollider2D.h"
#include "CGameObject.h"
#include "CRenderer.h"
#include "CScript.h"
#include "CCamera.h"
#include "CUiBase.h"

UINT Collider2D::ColliderNumber = 0;

Collider2D::Collider2D()
	: Component(eComponentType::Collider)
	, mType(eColliderType::None)
	, mTransform(nullptr)
	, mSize(Vector2::One)
	, mCenter(Vector2::Zero)
	, mbTrigger(false)
	, mID(0)
	, mRadius(0.0f)
{
	mID = ColliderNumber++;
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
	Vector3 size = mTransform->GetSize();
	scale = (size * scale) * mSize;

	Vector3 rotation = GetOwner()->GetComponent<Transform>()->GetRotation();
	Vector3 position = mTransform->GetPosition();
	Vector3 colliderPos = position + Vector3(mCenter.x, mCenter.y, 0.f);
	mPosition = colliderPos;

	/*Matrix scaleMatrix = Matrix::CreateScale(scale);

	Matrix rotationMatrix;
	rotationMatrix = Matrix::CreateRotationX(rotation.x);
	rotationMatrix *= Matrix::CreateRotationY(rotation.y);
	rotationMatrix *= Matrix::CreateRotationZ(rotation.z);

	Matrix positionMatrix;
	positionMatrix.Translation(Vector3(colliderPos.x, colliderPos.y, colliderPos.z));

	Matrix worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;*/

	DebugMesh meshAttricbute = {};
	meshAttricbute.position = Vector3(colliderPos.x, colliderPos.y, colliderPos.z);
	meshAttricbute.radius = mRadius;
	meshAttricbute.rotation = rotation;
	meshAttricbute.scale = scale;
	meshAttricbute.type = mType;
	meshAttricbute.LayerType = (UINT)(GetOwner()->GetLayerType());

	if (meshAttricbute.LayerType == (UINT)eLayerType::UI)
	{
		UiBase* ui = dynamic_cast<UiBase*>(GetOwner());
		meshAttricbute.renderAble = ui->GetIsAble();
		meshAttricbute.uiState = ui->GetPointToRect();
	}
	else
	{
		if (GetOwner()->GetState() == GameObject::eState::active)
			meshAttricbute.renderAble = true;
		else
			meshAttricbute.renderAble = false;
	}

	Renderer::debugMeshes.push_back(meshAttricbute);
}

void Collider2D::Render()
{
	if (GetOwner()->GetLayerType() == eLayerType::UI)
	{
		Matrix world = GetOwner()->GetComponent<Transform>()->GetWorldMatrix();
		Matrix view = Renderer::UiCamera->GetViewMatrix();
		Matrix projection = Renderer::UiCamera->GetProjectionMatrix();

		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();
		return;
	}
}

void Collider2D::OnCollisionEnter(Collider2D* collider)
{
	const std::vector<Script*>& scripts = GetOwner()->GetScripts();
	for (Script* script : scripts)
	{
		script->OnCollisionEnter(collider);
	}
}

void Collider2D::OnCollisionStay(Collider2D* collider)
{
	const std::vector<Script*>& scripts = GetOwner()->GetScripts();
	for (Script* script : scripts)
	{
		script->OnCollisionStay(collider);
	}
}

void Collider2D::OnCollisionExit(Collider2D* collider)
{
	const std::vector<Script*>& scripts = GetOwner()->GetScripts();
	for (Script* script : scripts)
	{
		script->OnCollisionExit(collider);
	}
}

void Collider2D::OnTriggerEnter(Collider2D* collider)
{
	const std::vector<Script*>& scripts = GetOwner()->GetScripts();
	for (Script* script : scripts)
	{
		script->OnTriggerEnter(collider);
	}
}

void Collider2D::OnTriggerStay(Collider2D* collider)
{
	const std::vector<Script*>& scripts = GetOwner()->GetScripts();
	for (Script* script : scripts)
	{
		script->OnTriggerStay(collider);
	}
}

void Collider2D::OnTriggerExit(Collider2D* collider)
{
	const std::vector<Script*>& scripts = GetOwner()->GetScripts();
	for (Script* script : scripts)
	{
		script->OnTriggerExit(collider);
	}
}
