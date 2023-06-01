#include "CWallObject.h"
#include "CResourceManager.h"
#include "CMeshRenderer.h"
#include "CWorldManager.h"
#include "CObjectManager.h"
#include "CInput.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"
#include "CObject.h"

#include "..//Dx11_Engine/GuiEditor.h"
#include "..//Dx11_Engine/GuiGame.h"
#include "..//Dx11_Engine/GuiInspector.h"

extern gui::Editor _Editor;

using namespace graphics;

static int id = 0;

WallObject::WallObject()
	: GameObject()
	, mWidthHeight(Vector2(300.f,300.f))
	, mOffset(Vector2::Zero)
	, mAlpha(1.0f)
	, mbUpdate(true)
	, mWallType(eWallType::End)
{
	SetName(L"WallObject" + to_wstring(id));
	id++;
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
	Vector3 mSize = Vector3(200.f, 100.f, 1.0f);

	Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos();

	if (_Editor.GetActive())
		mousePos = _Editor.GetEditorWorldMousePos();


	if (mPos.x - TILE_X_HALF_SIZE > mousePos.x || mPos.x + TILE_X_HALF_SIZE < mousePos.x)
		return;

	if (mPos.y - TILE_Y_HALF_SIZE > mousePos.y || mPos.y + TILE_Y_HALF_SIZE < mousePos.y)
		return;

	// 기울기
	float fslope = (100.f * 0.5f) / (200.f * 0.5f);
	float fSlope[4] =
	{
		fslope,
		-fslope,
		fslope,
		-fslope,
	};

	// 마름모 정점
	Vector2 vVertex[4] =
	{
		{mPos.x, mPos.y + TILE_Y_HALF_SIZE},
		{mPos.x + TILE_X_HALF_SIZE, mPos.y},
		{ mPos.x, mPos.y - TILE_Y_HALF_SIZE},
		{ mPos.x - TILE_X_HALF_SIZE, mPos.y},
	};

	// 절편
	float fY_Intercept[4] =
	{
		vVertex[0].y - (fSlope[0] * vVertex[0].x),
		vVertex[1].y - (fSlope[1] * vVertex[1].x),
		vVertex[2].y - (fSlope[2] * vVertex[2].x),
		vVertex[3].y - (fSlope[3] * vVertex[3].x),
	};

	//0 = ax + b - y;

	float tset[4] =
	{
		fSlope[0] * mousePos.x + fY_Intercept[0] - mousePos.y,
		fSlope[1] * mousePos.x + fY_Intercept[1] - mousePos.y,
		fSlope[2] * mousePos.x + fY_Intercept[2] - mousePos.y,
		fSlope[3] * mousePos.x + fY_Intercept[3] - mousePos.y
	};

	if (0 < fSlope[0] * mousePos.x + fY_Intercept[0] - mousePos.y &&
		0 < fSlope[1] * mousePos.x + fY_Intercept[1] - mousePos.y &&
		0 > fSlope[2] * mousePos.x + fY_Intercept[2] - mousePos.y &&
		0 > fSlope[3] * mousePos.x + fY_Intercept[3] - mousePos.y)
	{
		if (Input::GetInstance()->GetKeyPress(eKeyCode::LBTN))
		{

		}
		else if (Input::GetInstance()->GetKeyPress(eKeyCode::RBTN))
		{
			Object::ObjectDestroy(this);
		}
		else if (Input::GetInstance()->GetKeyDown(eKeyCode::R) && _Editor.GetActive())
		{
			Renderer::InspectorGameObject = this;

			gui::Inspector* inspector = _Editor.GetWidget<gui::Inspector>("Inspector");
			inspector->SetTargetGameObject(Renderer::InspectorGameObject);
			inspector->InitalizeTargetGameObject();
		}

		mbOnObject = true;
		_Editor.GetWidget<gui::Game>("Game")->SetCreateObject(false);
	}

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
	/*if (mbUpdate == false)
		return;*/

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