#include "CPlayScene.h"
#include "CTexture2D.h"
#include "CpaintShader.h"

// Object
#include "CTileObject.h"
#include "CHUD.h"
#include "CBackGround.h"
#include "CPlayer.h"
#include "CMinosTauros.h"

// Static
#include "CObject.h"
#include "CRenderer.h"
// Component
#include "CMeshRenderer.h"
#include "CAnimator.h"
#include "CLight.h"
#include "CSpriteRenderer.h"
#include "CParticleSystem.h"
#include "CCollider2D.h"
#include "CGridScript.h"
#include "CCameraScript.h"
#include "CPlayerScript.h"

// Manager
#include "CResourceManager.h"
#include "CInput.h"
#include "CCollisionManager.h"
#include "CWorldManager.h"
using namespace graphics;

PlayScene::PlayScene()
	: Scene(eSceneType::Play)
{

}
PlayScene::~PlayScene()
{

}

void PlayScene::Initalize()
{
	// PaintShader
	
		//shared_ptr<PaintShader> paintShader = ResourceManager::GetInstance()->Find<PaintShader>(L"PaintShader");
		//shared_ptr<graphics::Texture2D> paintTex = ResourceManager::GetInstance()->Find<graphics::Texture2D>(L"PaintTexture");

		//paintShader->SetTarget(paintTex);
		//paintShader->OnExcute();
	

	{
		GameObject* directionalLight = Object::Instantiate<GameObject>(eLayerType::None, this);
		Transform* tr = directionalLight->GetComponent<Transform>();
		tr->SetPosition(Vector3(0.0f, 0.0f, -100.f));
		Light* lightcomp = directionalLight->AddComponent<Light>();
		lightcomp->SetType(eLightType::Directional);
		lightcomp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// IsoMetric
	{
		GameObject* IsoMetricCamera = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		IsoMetricCamera->GetComponent<Transform>()->SetPosition(Vector3(10.f, 10.f, 10.f));
		IsoMetricCamera->GetComponent<Transform>()->SetRotation(Vector3(0.f, 0.f, -0.f));
		Camera* IsoMetricCameraComp = IsoMetricCamera->AddComponent<Camera>();
		IsoMetricCameraComp->DisableLayerMasks();
		IsoMetricCameraComp->TurnLayerMask(eLayerType::Tile, true);

		IsoMetricCameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		mIsometricCamera = IsoMetricCameraComp;
		Renderer::IsometricCamera = IsoMetricCameraComp;
	}

	// Main Camera Game Object
	{
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(10.0f, 10.f, 1.0f));
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraComp->TurnLayerMask(eLayerType::Tile, false);
		//cameraComp->TurnLayerMask(eLayerType::Tile, false);
		cameraObj->AddComponent<CameraScript>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		Renderer::mainCamera = cameraComp;
		mMainCamera = cameraComp;

		mIsometricCamera->SetTrace(GetMainCam()->GetOwner());
	}
	// Ui Camera
	{
		GameObject* uiCamera = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* uiCameraComp = uiCamera->AddComponent<Camera>();
		uiCameraComp->DisableLayerMasks();
		uiCameraComp->TurnLayerMask(eLayerType::UI, true);

		uiCameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		Renderer::UiCamera = uiCameraComp;
		mUiCamera = uiCameraComp;
	}

	{
		/*BackGround* bg = Object::Instantiate<BackGround>(eLayerType::BackGround, this);
		SpriteRenderer* sr = bg->AddComponent<SpriteRenderer>();
		weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"FadeMesh");
		weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"Rect2Material");
		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->
			Load<Texture2D>(L"ItemSlot", L"UI//NoneCanvers.png");

		sr->SetMesh(mesh);
		sr->SetMaterial(material);
		material.lock()->SetTexture(eTextureSlot::T0, tex);

		Transform* tr = bg->GetComponent<Transform>();
		tr->SetScale(Vector3(10000.f, 10000.f, 1.0f));*/
	}
	/*{
		GameObject* fade = Object::Instantiate<GameObject>(eLayerType::BackGround, this);
		SpriteRenderer* sr = fade->AddComponent<SpriteRenderer>();
		shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"FadeMesh");
		shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"AstarMaterial");

		sr->SetMesh(mesh);
		sr->SetMaterial(material);

		Transform* tr = fade->GetComponent<Transform>();
		tr->SetScale(Vector3(10.f, 10.f, 1.0f));
		tr->SetPosition(Vector3(10.f, 10.f, 0.9f));
	}*/

	GameObject* obj = nullptr;
	// Player
	{
		/*Player* player = Object::Instantiate<Player>(eLayerType::Player, this);
		PlayerScript* sc = player->AddComponent<PlayerScript>();

		Collider2D* collider = player->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = player->GetComponent<Transform>();
		tr->SetPosition(Vector3(10.0f, 10.0f, 1.0f));
		tr->SetScale(Vector3(3.0f, 3.0f, 1.0f));

		SpriteRenderer* spr = player->AddComponent<SpriteRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");

		spr->SetMesh(mesh);
		spr->SetMaterial(material);

		player->InitAnimation();
		player->PlayAnimation(L"Walk0");

		Renderer::mainCamera->SetTrace(player);
		obj = player;

		Renderer::InspectorGameObject = player;

		WorldManager::GetInstance()->SetPlayer(player);*/
		//60 75.875f
	}

	{
		MinosTauros* mino = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr = mino->GetComponent<Transform>();
		minoTr->SetPosition(Vector3(10.f, 10.f, 1.0f));
		minoTr->SetScale(Vector3(3.f, 3.f, 1.0f));
		mino->SetTarget(obj);
	}

	// PostProcess
	{
		/*Player* player = Object::Instantiate<Player>(eLayerType::Player, this);
		PlayerScript* sc = player->AddComponent<PlayerScript>();

		Collider2D* collider = player->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = player->GetComponent<Transform>();
		tr->SetPosition(Vector3(10.0f, 10.0f, 1.0f));
		tr->SetScale(Vector3(30.0f, 30.0f, 1.0f));

		SpriteRenderer* spr = player->AddComponent<SpriteRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"PostProcessMaterial");

		spr->SetMesh(mesh);
		spr->SetMaterial(material);

		player->InitAnimation();
		player->PlayAnimation(L"Walk0");*/
	}

	// tile
	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(11.0f, 11.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater= ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	int indexX = 0;
	int indexY = 0;
	{
		bool a = false;
		float sizeX = 200.f;
		float sizeY = 100.f;
		for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				if (x + y >= 20)
					continue;

				TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);
				Transform* tr = tile->GetComponent<Transform>();
				UINT maxX = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxX();
				UINT maxY = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxY();

				tile->SetMaxIndex(maxX, maxY);
				tile->SetIndex(x, y);
				
				tr->SetPosition(Vector3((x*(sizeX * 0.5f) + 5000.f), (y* sizeY) + (x * (sizeY * 0.5f)) + 5000.f, -50.0f));
				tr->SetSize(Vector3(sizeX, sizeY, 1.0f));

				indexX = x;
				indexY = y;

				tile->SetScreenIndex(x, y);

			/*	if (!a)
				{
					float xt = (x * (sizeX * 0.5f) + 5000.f) - (sizeX * 0.75f);
					float yt = ((y * sizeY) + (x * (sizeY * 0.5f)) + 5000.f) - (sizeY * 0.75f);
					tr->SetPosition(Vector3((x* (sizeX * 0.5f) + 5000.f) - (sizeX * 0.75f), ((y* sizeY) + (x * (sizeY * 0.5f)) + 5000.f) - (sizeY * 0.75f), -50.0f));

					tile->SetIndex(x, y);
					a = true;
				}*/
			}
		}

		/*for (int y = 0; y < 20; ++y)
		{
			for (int x = 0; x < 20; ++x)
			{
				if (x + y >= 20)
					continue;

				TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);
				Transform* tr = tile->GetComponent<Transform>();
				UINT maxX = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxX();
				UINT maxY = tile->GetMaterial()->GetTexture(eTextureSlot::T0).lock()->GetMaxY();

				tile->SetMaxIndex(maxX, maxY);
				tile->SetIndex(x, y);


				tr->SetPosition(Vector3((- 1 * x) * (sizeX * 0.5f) + 5000.f, (y* sizeY) + (x * (sizeY * 0.5f)) + 5000.f, -50.0f));
				tr->SetSize(Vector3(sizeX, sizeY, 1.0f));

				tile->SetScreenIndex(x + indexX, y + indexY);
			}
		}*/
	}

	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(12.0f, 11.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(13.0f, 11.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(14.0f, 11.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(15.0f, 11.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(16.0f, 11.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(16.0f, 10.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(16.0f, 9.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(16.0f, 8.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(16.0f, 7.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	//{
	//	TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//	/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//	collider->SetSize(Vector2(0.5f, 0.5f));
	//	collider->SetType(eColliderType::Rect);*/

	//	Transform* tr = tile->GetComponent<Transform>();
	//	tr->SetPosition(Vector3(16.0f, 6.0f, 1.0f));
	//	tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//	MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//	mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(mater);

	//	WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//}

	//for (int i = 1; i < 6; ++i)
	//{
	//	{
	//		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

	//		/*Collider2D* collider = tile->AddComponent<Collider2D>();
	//		collider->SetSize(Vector2(0.5f, 0.5f));
	//		collider->SetType(eColliderType::Rect);*/

	//		Transform* tr = tile->GetComponent<Transform>();
	//		tr->SetPosition(Vector3(16.0f + i, 6.0f, 1.0f));
	//		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

	//		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
	//		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	//		std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//		std::weak_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

	//		mater.lock()->SetTexture(eTextureSlot::T0, tex);

	//		mr->SetMesh(mesh);
	//		mr->SetMaterial(mater);

	//		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	//	}
	//}
	//Particle
	/*{
		GameObject* obj = Object::Instantiate<GameObject>(eLayerType::Particle, this);
		obj->SetName(L"Particle");

		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(Vector3(11.0f, 0.0f, 100.0f));

		obj->AddComponent<ParticleSystem>();
	}*/

	Scene::Initalize();
}

void PlayScene::Update()
{
	Scene::Update();
}

void PlayScene::FixedUpdate()
{
	Scene::FixedUpdate();
}

void PlayScene::Render()
{
	Scene::Render();
}

void PlayScene::OnEnter()
{
	if (mMainCamera)
		Renderer::mainCamera = mMainCamera;

	if(mUiCamera)
		Renderer::UiCamera = mUiCamera;

	GameObject* player = WorldManager::GetInstance()->GetPlayer();

	if (player == nullptr)
		return;

	Renderer::mainCamera->SetTrace(player);
	Renderer::InspectorGameObject = player;

	this->GetLayer(eLayerType::Player).AddGameObject(player);
}

void PlayScene::OnExit()
{
	
}