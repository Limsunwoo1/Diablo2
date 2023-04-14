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

	// Main Camera Game Object
	{
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(10.0f, 10.f, 1.0f));
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		Renderer::mainCamera = cameraComp;
		mMainCamera = cameraComp;
	}
	// Ui Camera
	{
		GameObject* uiCamera = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* uiCameraComp = uiCamera->AddComponent<Camera>();
		uiCameraComp->DisableLayerMasks();
		uiCameraComp->TurnLayerMask(eLayerType::UI, true);

		uiCameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
	}

	// Player
	{
		Player* player = Object::Instantiate<Player>(eLayerType::Player, this);
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

		//60 75.875f
	}

	{
		MinosTauros* mino = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr = mino->GetComponent<Transform>();
		minoTr->SetPosition(Vector3(10.f, 10.f, 1.0f));
		minoTr->SetScale(Vector3(3.f, 3.f, 1.0f));
	}

	// tile
	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(11.0f, 11.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater= ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(12.0f, 11.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(13.0f, 11.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(14.0f, 11.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(15.0f, 11.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(16.0f, 11.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(16.0f, 10.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(16.0f, 9.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(16.0f, 8.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(16.0f, 7.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	{
		TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

		Collider2D* collider = tile->AddComponent<Collider2D>();
		collider->SetSize(Vector2(0.5f, 0.5f));
		collider->SetType(eColliderType::Rect);

		Transform* tr = tile->GetComponent<Transform>();
		tr->SetPosition(Vector3(16.0f, 6.0f, 1.0f));
		tr->SetRotation(Vector3(0.f, 0.f, -45.f));

		MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

		mater->SetTexture(eTextureSlot::T0, tex);

		mr->SetMesh(mesh);
		mr->SetMaterial(mater);

		WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
	}

	for (int i = 1; i < 6; ++i)
	{
		{
			TileObject* tile = Object::Instantiate<TileObject>(eLayerType::Tile, this);

			Collider2D* collider = tile->AddComponent<Collider2D>();
			collider->SetSize(Vector2(0.5f, 0.5f));
			collider->SetType(eColliderType::Rect);

			Transform* tr = tile->GetComponent<Transform>();
			tr->SetPosition(Vector3(16.0f + i, 6.0f, 1.0f));
			tr->SetRotation(Vector3(0.f, 0.f, -45.f));

			MeshRenderer* mr = tile->AddComponent<MeshRenderer>();
			std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
			std::shared_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
			std::shared_ptr<Texture2D>tex = ResourceManager::GetInstance()->Find<Texture2D>(L"ShopIdel");

			mater->SetTexture(eTextureSlot::T0, tex);

			mr->SetMesh(mesh);
			mr->SetMaterial(mater);

			WorldManager::GetInstance()->SetObstacle(tr->GetPosition().x, tr->GetPosition().y);
		}
	}
	//Particle
	{
		GameObject* obj = Object::Instantiate<GameObject>(eLayerType::Particle, this);
		obj->SetName(L"Particle");

		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(Vector3(11.0f, 0.0f, 100.0f));

		obj->AddComponent<ParticleSystem>();
	}

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
}

void PlayScene::OnExit()
{

}