#include "CPing.h"
#include "CTime.h"
#include "CRenderer.h"
#include "CInput.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CMeshRenderer.h"
#include "CObject.h"

Ping::Ping()
	: GameObject()
	, mSize(0.7f)
	, mTime(0.0f)
	, mPos(Vector2::Zero)
{
}

Ping::~Ping()
{
}

void Ping::Initalize()
{
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"PingMaterial");

	sr->SetMesh(mesh);
	sr->SetMaterial(mater);
}

void Ping::Update()
{
	mTime += Time::GetInstance()->DeltaTime();
	GameObject::Update();
}

void Ping::FixedUpdate()
{
	if (mTime >= 0.5f)
	{
		Object::ObjectDestroy(this);
		return;
	}

	// Size
	Transform* tr = GetComponent<Transform>();
	Vector3 size = tr->GetScale();
	float inSize = mSize * (mTime * 3.f);

	if (inSize > 0.7f)
		inSize = 0.7f;

	size.x = inSize;
	size.y = inSize;
	size.z = 1.0f;

	tr->SetScale(size);
	Vector3 pos = tr->GetPosition();

	// TextureSet
	SpriteRenderer* sr = GetComponent<SpriteRenderer>();
	std::shared_ptr<Material> mater = sr->GetMaterial();
	std::shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Ping");
	mater->SetTexture(eTextureSlot::T0, tex);

	// Bind Time
	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Time];
	Renderer::TimeCB info = {};
	info.deltatime = Time::GetInstance()->DeltaTime();
	info.time = mTime * 2.f;

	cb->SetData(&info);
	cb->Bind(eShaderStage::PS);

	GameObject::FixedUpdate();
}

void Ping::Render()
{
	GameObject::Render();
}

void Ping::InitAnimation()
{
}
