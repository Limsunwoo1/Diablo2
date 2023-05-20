#include "CTilePallet.h"
#include "CMeshRenderer.h"
#include "CResourceManager.h"

TilePallet::TilePallet()
{
}

TilePallet::~TilePallet()
{
}

void TilePallet::Initalize()
{
	Transform* tr = GetComponent<Transform>();
	tr->SetScale(Vector3(32.f, 16.f, 0.f));
	tr->SetPosition(Vector3(0.0f, 0.0f, 10.f));


	MeshRenderer* mr = AddComponent<MeshRenderer>();
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"IsoMetricMesh");
	std::weak_ptr<Material> material= ResourceManager::GetInstance()->Find<Material>(L"Grid2Material");

	mr->SetMesh(mesh);
	mr->SetMaterial(material);
}

void TilePallet::Update()
{
	GameObject::Update();
}

void TilePallet::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void TilePallet::Render()
{
	GameObject::Render();
}
