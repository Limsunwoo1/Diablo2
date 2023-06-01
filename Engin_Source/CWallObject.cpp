#include "CWallObject.h"
#include "CResourceManager.h"
#include "CMeshRenderer.h"
#include "CWorldManager.h"
#include "CObjectManager.h"

using namespace graphics;

WallObject::WallObject()
	: GameObject()
	, mWidthHeight(Vector2::Zero)
	, mOffset(Vector2::Zero)
	, mAlpha(1.0f)
	, mbUpdate(true)
	, mWallType(eWallType::End)
{
}

WallObject::~WallObject()
{
	ResourceManager::GetInstance()->Delete<Material>(GetName());
}

void WallObject::Initalize()
{
	// MeshRenderer
	MeshRenderer* mr = AddComponent<MeshRenderer>();

	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> mater = std::shared_ptr<Material>();

	mr->SetMesh(mesh);
	mr->SetMaterial(mater);

	wstring name = GetName();
	ResourceManager::GetInstance()->Insert<Material>(name, mater);

	mMaterial = mater;
}

void WallObject::Update()
{
	if (mTexture.lock() == nullptr)
		return;

	GameObject* player = WorldManager::GetInstance()->GetPlayer();
	if (player == nullptr)
		return;

	Transform* playertr = player->GetComponent<Transform>();
	Transform* mTr = GetComponent<Transform>();

	Vector2 mPos = Vector2(mTr->GetPosition().x, mTr->GetPosition().y);
	mPos += mOffset;

	Vector2 PlayerPos = Vector2(playertr->GetPosition().x, playertr->GetPosition().y);


	// 플리이어와의 거리별 알파값 세팅
	float distance = Vector2(mPos - PlayerPos).Length();

	int maxDist = 10;
	mAlpha = distance / maxDist;

	if (mAlpha >= 1.0f)
		mAlpha = 1.0f;
}

void WallObject::FixedUpdate()
{
	if (mbUpdate == false)
		return;

	if (mTexture.lock() == nullptr)
		return;

	GameObject::FixedUpdate();
	mbUpdate = false;
}

void WallObject::Render()
{
	if (mTexture.lock() == nullptr)
		return;

	if (mMaterial.lock()->GetTexture(eTextureSlot::T0).lock().get() == nullptr)
	{
		mMaterial.lock()->SetTexture(eTextureSlot::T0, mTexture);
	}

	if (mWidthHeight != Vector2::Zero)
	{
		Transform* Tr = GetComponent<Transform>();
		Tr->SetSize(Vector3(mWidthHeight.x, mWidthHeight.y, 1.0f));
	}
}

void WallObject::SetWallType(eWallType type)
{
	mWallType = type;
	mOffset = ObjectManager::GetInstance()->GetOffsetData(mWallType);
}

void WallObject::SetTexture(std::weak_ptr<graphics::Texture2D> tex)
{
	mTexture = tex;
}
