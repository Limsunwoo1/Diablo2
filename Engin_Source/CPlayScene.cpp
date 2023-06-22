#include "CPlayScene.h"
#include "CTexture2D.h"
#include "CpaintShader.h"
#include "CSceneManager.h"
#include "CToolScene.h"
#include "CTilePallet.h"

// Object
#include "CTileObject.h"
#include "CHUD.h"
#include "CBackGround.h"
#include "CPlayer.h"
#include "CMinosTauros.h"
#include "CSpearMonster.h"
#include "CRadamentMonster.h"
#include "CMephistoMonster.h"
#include "CAndarielMonster.h"
#include "CTriggerObject.h"
#include "CItemManager.h"
#include "CDiabloMonster.h"

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
		/*GameObject* IsoMetricCamera = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		IsoMetricCamera->GetComponent<Transform>()->SetPosition(Vector3(10.f, 10.f, 10.f));
		IsoMetricCamera->GetComponent<Transform>()->SetRotation(Vector3(0.f, 0.f, -0.f));
		Camera* IsoMetricCameraComp = IsoMetricCamera->AddComponent<Camera>();
		IsoMetricCameraComp->DisableLayerMasks();
		IsoMetricCameraComp->TurnLayerMask(eLayerType::Tile, true);

		IsoMetricCameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		mIsometricCamera = IsoMetricCameraComp;
		Renderer::IsometricCamera = IsoMetricCameraComp;*/
	}

	// Main Camera Game Object
	{
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(10.0f, 10.f, 1.0f));
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		//cameraComp->TurnLayerMask(eLayerType::Tile, false);
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
		TriggerObject* trigger = Object::Instantiate<TriggerObject>(eLayerType::Gate, this);
		Transform* TGTr = trigger->GetComponent<Transform>();
		TGTr->SetPosition(Vector3(3610.f, 9470.f, 1.0f));
		TGTr->SetSize(Vector3(400.f, 250.f, 1.0f));

		trigger->GetFunc() = std::bind(&PlayScene::OnEvent,this);
	}

	AndarielMonster* mino1 = Object::Instantiate<AndarielMonster>(eLayerType::Monster, this);
	Transform* minoTr1 = mino1->GetComponent<Transform>();
	minoTr1->SetPosition(Vector3(5655.f, 9701.f, 1.0f));
	mino1->Paused();

	Boss = mino1;

	{
		SpearMonster* monster = Object::Instantiate<SpearMonster>(eLayerType::Monster, this);
		Transform* spearTr = monster->GetComponent<Transform>();
		spearTr->SetPosition(Vector3(5985.f,7257.f, 1.0f));

		SpearMonster* mino = Object::Instantiate<SpearMonster>(eLayerType::Monster, this);
		Transform* minoTr = mino->GetComponent<Transform>();
		minoTr->SetPosition(Vector3(5690.f, 7418.f, 1.0f));

		SpearMonster* mino2 = Object::Instantiate<SpearMonster>(eLayerType::Monster, this);
		Transform* minoTr2 = mino2->GetComponent<Transform>();
		minoTr2->SetPosition(Vector3(5550.f, 7170.f, 1.0f));

		SpearMonster* mino3 = Object::Instantiate<SpearMonster>(eLayerType::Monster, this);
		Transform* minoTr3 = mino3->GetComponent<Transform>();
		minoTr3->SetPosition(Vector3(5332.f, 7380.f, 1.0f));

		RadamentMonster* mino4 = Object::Instantiate<RadamentMonster>(eLayerType::Monster, this);
		Transform* minoTr4 = mino4->GetComponent<Transform>();
		minoTr4->SetPosition(Vector3(5431, 7357.f, 1.0f));
	}

	{
		SpearMonster* monster = Object::Instantiate<SpearMonster>(eLayerType::Monster, this);
		Transform* spearTr = monster->GetComponent<Transform>();
		spearTr->SetPosition(Vector3(4141.f, 9047.f, 1.0f));

		SpearMonster* mino = Object::Instantiate<SpearMonster>(eLayerType::Monster, this);
		Transform* minoTr = mino->GetComponent<Transform>();
		minoTr->SetPosition(Vector3(4152.f, 8916.f, 1.0f));

		SpearMonster* mino2 = Object::Instantiate<SpearMonster>(eLayerType::Monster, this);
		Transform* minoTr2 = mino2->GetComponent<Transform>();
		minoTr2->SetPosition(Vector3(4392.f, 8929, 1.0f));

		SpearMonster* mino3 = Object::Instantiate<SpearMonster>(eLayerType::Monster, this);
		Transform* minoTr3 = mino3->GetComponent<Transform>();
		minoTr3->SetPosition(Vector3(4562.f, 8890.f, 1.0f));

		RadamentMonster* mino4 = Object::Instantiate<RadamentMonster>(eLayerType::Monster, this);
		Transform* minoTr4 = mino4->GetComponent<Transform>();
		minoTr4->SetPosition(Vector3(4568.f, 8843.f, 1.0f));

		RadamentMonster* mino5 = Object::Instantiate<RadamentMonster>(eLayerType::Monster, this);
		Transform* minoTr5 = mino5->GetComponent<Transform>();
		minoTr5->SetPosition(Vector3(4072.f, 9045.f, 1.0f));
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

	if(mUiCamera)
		Renderer::UiCamera = mUiCamera;

	GameObject* player = WorldManager::GetInstance()->GetPlayer();
	player->DontDestroy(true);
	player->GetComponent<Transform>()->SetPosition(Vector3(5000.f, 6850.f,1.0f));

	if (player == nullptr)
		return;

	Renderer::mainCamera->SetTrace(player);
	Renderer::InspectorGameObject = player;

	this->GetLayer(eLayerType::Player).AddGameObject(player);

	ToolScene* tool = dynamic_cast<ToolScene*>(SceneManager::GetInstance()->GetScene(eSceneType::Tool));
	if (tool == nullptr)
		return;

	TilePallet* pallet = tool->GetTilePallet();
	if (pallet == nullptr)
		return;

	pallet->Load(L"..//Resource//TileData//play",eSceneType::Play);

	// collision
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::PlayerSKil, eLayerType::Monster);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::MonsterSkil);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::PlayerSKil, eLayerType::Wall);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::MonsterSkil, eLayerType::Wall);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::Gate);
	
}

void PlayScene::OnExit()
{
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::PlayerSKil, eLayerType::Monster, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::MonsterSkil, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::PlayerSKil, eLayerType::Wall, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::MonsterSkil, eLayerType::Wall, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::Gate, false);

	GameObject* obj = WorldManager::GetInstance()->GetPlayer();
	Player* player = dynamic_cast<Player*>(obj);
	SceneManager::GetInstance()->GetScene(eSceneType::Play)->GetLayer(eLayerType::Player).EreaseObj(player);
	player->SetState(Player::PlayerState::Idle);

	ItemManager::GetInstance()->ClearWorldItem();
}

void PlayScene::OnEvent()
{
	/*AndarielMonster* mino1 = Object::Instantiate<AndarielMonster>(eLayerType::Monster, this);
	Transform* minoTr1 = mino1->GetComponent<Transform>();
	minoTr1->SetPosition(Vector3(5655.f, 9701.f, 1.0f));*/

	if (Boss)
		Boss->Active();
}
