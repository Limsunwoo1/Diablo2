#include "CWallObject.h"
#include "CResourceManager.h"
#include "CMeshRenderer.h"
#include "CWorldManager.h"
#include "CObjectManager.h"
#include "CInput.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"

using namespace graphics;

WallObject::WallObject()
	: GameObject()
	, mWidthHeight(Vector2(300.f,300.f))
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
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(300.f, 300.f,1.0f));
	// MeshRenderer
	MeshRenderer* mr = AddComponent<MeshRenderer>();

	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"ObjectShader");
	std::shared_ptr<Material> mater = std::make_shared<Material>();
	mater->SetRenderingMode(graphics::eRenderingMode::Transparent);

	mr->SetMesh(mesh);
	mr->SetMaterial(mater);

	wstring name = GetName();
	ResourceManager::GetInstance()->Insert<Material>(name, mater);

	mMaterial = mater;
	mater->SetShader(shader);
}

void WallObject::Update()
{
	mbOnObject = false;
	if (mTexture.lock() == nullptr)
		return;

	Transform* mTr = GetComponent<Transform>();
	Vector2 mPos = Vector2(mTr->GetPosition().x, mTr->GetPosition().y);
	mPos += mOffset + ObjectManager::GetInstance()->GetOffsetData(mWallType);
	Vector3 mSize = mTr->GetScale() * mTr->GetSize();

	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos();
	if (mPos.x - (mSize.x * 0.5f) > mousePos.x || mPos.x + (mSize.x * 0.5f) < mousePos.x)
		return;

	if (mPos.y - (mSize.y * 0.5f) > mousePos.y || mPos.y + (mSize.y * 0.5f) < mousePos.y)
		return;

	mbOnObject = true;

	GameObject* player = WorldManager::GetInstance()->GetPlayer();
	if (player == nullptr)
		return;

	GameObject::Update();

	Transform* playertr = player->GetComponent<Transform>();
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

	Renderer::ObjectDataCB info = {};
	info.OnObject = mbOnObject;
	info.ObjectAlpha = mAlpha;

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::ObjectData];
	cb->SetData(&info);
	cb->Bind(eShaderStage::PS);
	
	GameObject::Render();
}

void WallObject::SetWidthHeigth(Vector2 size)
{
	mWidthHeight = size;

	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(size.x, size.y, 1.0f));
}

void WallObject::SetOffset(Vector2 offset)
{
	mOffset = offset;

	Transform* tr = GetComponent<Transform>();
	Vector2 defaultOffset = ObjectManager::GetInstance()->GetOffsetData(mWallType);
	tr->SetOffset(Vector3(mOffset.x + defaultOffset.x, mOffset.y + defaultOffset.y, 0.0f));
}

void WallObject::SetWallType(eWallType type)
{
	mWallType = type;
}