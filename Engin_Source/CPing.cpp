#include "CPing.h"
#include "CTime.h"
#include "CRenderer.h"
#include "CInput.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CMeshRenderer.h"

Ping::Ping()
	: GameObject()
	, mSize(1.0f)
	, mTime(0.0f)
	, mPos(Vector2::Zero)
{
	Initalize();
}

Ping::~Ping()
{
}

void Ping::Initalize()
{
	MeshRenderer* sr = AddComponent<MeshRenderer>();
	std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

	sr->SetMesh(mesh);
	sr->SetMaterial(mater);
}

void Ping::Update()
{
	GameObject::Update();
	mTime += Time::GetInstance()->DeltaTime();
}

void Ping::FixedUpdate()
{
	/*if (mTime >= 1.0f)
	{
		mTime = 0.0f;
		Death();
		return;
	}*/

	// Size
	Transform* tr = GetComponent<Transform>();
	Vector3 size = tr->GetScale();
	float inSize = mSize;// *mTime;

	size.x = inSize;
	size.y = inSize;
	size.z = 1.0f;

	tr->SetScale(size);
	Vector3 pos = tr->GetPosition();
	cout << "X   " << pos.x << "    Y     " << pos.y << endl;
	//tr->SetPosition(Vector3(mPos.x, mPos.y, 1.0f));

	// TextureSet
	MeshRenderer* sr = GetComponent<MeshRenderer>();
	std::shared_ptr<Material> mater = sr->GetMaterial();
	std::shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"Ping");
	mater->SetTexture(eTextureSlot::T0, tex);
	

	// Bind Time
	/*ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Time];
	Renderer::TimeCB info = {};
	info.deltatime = Time::GetInstance()->DeltaTime();
	info.time = mTime;

	cb->SetData(&info);
	cb->Bind(eShaderStage::PS);*/

	GameObject::FixedUpdate();
}

void Ping::Render()
{
	GameObject::Render();
}

void Ping::InitAnimation()
{
}
