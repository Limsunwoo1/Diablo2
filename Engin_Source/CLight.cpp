#include "CLight.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CRenderer.h"

Light::Light()
	: Component(eComponentType::Light)
{

}

Light::~Light()
{

}

void Light::Initalize()
{

}

void Light::Update()
{

}

void Light::FixedUpdate()
{
	Transform* tr = GetOwner()->GetComponent<Transform>();
	Vector3 position = tr->GetPosition();
	mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
	mAttribute.Direction = Vector4(tr->Right().x, tr->Right().y, tr->Right().z, 1.0f);

	Renderer::lights.push_back(mAttribute);
}

void Light::Render()
{

}
