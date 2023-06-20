#include "CPlaySCene2.h"
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
#include "CUIManager.h"
#include "CUiBase.h"
#include "CInventoryPanel.h"
#include "CMainPanel.h"
#include "CEquipmentButton.h"
#include "CItemManager.h"

using namespace graphics;

PlayScene2::PlayScene2()
	: Scene(eSceneType::Play2)
{

}
PlayScene2::~PlayScene2()
{

}

void PlayScene2::Initalize()
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
		TriggerObject* trigger = Object::Instantiate<TriggerObject>(eLayerType::Gate, this);
		Transform* TGTr = trigger->GetComponent<Transform>();
		TGTr->SetPosition(Vector3(4273.f, 11493.f, 1.0f));
		TGTr->SetSize(Vector3(300.f, 300.f, 1.0f));

		trigger->GetFunc() = std::bind(&PlayScene2::OnEvent, this);
	}

	Scene::Initalize();
}

void PlayScene2::Update()
{
	Scene::Update();
}

void PlayScene2::FixedUpdate()
{
	Scene::FixedUpdate();
}

void PlayScene2::Render()
{
	Scene::Render();
}

void PlayScene2::OnEnter()
{
	if (mMainCamera)
		Renderer::mainCamera = mMainCamera;

	if (mUiCamera)
		Renderer::UiCamera = mUiCamera;

	GameObject* player = WorldManager::GetInstance()->GetPlayer();

	if (player == nullptr)
		return;

	Transform* tr = player->GetComponent<Transform>();
	tr->SetPosition(Vector3(4567.f, 7567.f, 1.0f));

	player->GetScript<PlayerScript>()->ResetAStar();
	player->GetComponent<AStar>()->StopRun();

	Renderer::mainCamera->SetTrace(player);
	Renderer::InspectorGameObject = player;

	this->GetLayer(eLayerType::Player).AddGameObject(player);

	ToolScene* tool = dynamic_cast<ToolScene*>(SceneManager::GetInstance()->GetScene(eSceneType::Tool));
	if (tool == nullptr)
		return;

	TilePallet* pallet = tool->GetTilePallet();
	if (pallet == nullptr)
		return;

	pallet->Load(L"..//Resource//TileData//lavaMap", eSceneType::Play);

	// collision
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::PlayerSKil, eLayerType::Monster);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::MonsterSkil);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::PlayerSKil, eLayerType::Wall);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::MonsterSkil, eLayerType::Wall);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::Gate);

	UiBase* invenpanel = UIManager::GetInstance()->GetUiInstance<InventoryPanel>(L"mainInventory");
	UiBase* mainpanel = UIManager::GetInstance()->GetUiInstance<MainPanel>(L"mainPanel");
	UiBase* monsterHp = UIManager::GetInstance()->GetUiInstance<Panel>(L"MonsterHp");

	Object::Instantiate<InventoryPanel>(eLayerType::UI, invenpanel, true, eSceneType::Play2);
	Object::Instantiate<MainPanel>(eLayerType::UI, mainpanel, true, eSceneType::Play2);
	Object::Instantiate<Panel>(eLayerType::UI, monsterHp, true, eSceneType::Play2);

	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::PlayerSKil, eLayerType::Monster, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::MonsterSkil, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::PlayerSKil, eLayerType::Wall, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::MonsterSkil, eLayerType::Wall, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::Gate, false);
}

void PlayScene2::OnExit()
{
	ItemManager::GetInstance()->ClearWorldItem();
}

void PlayScene2::OnEvent()
{
	DiabloMonster* diablo = Object::Instantiate<DiabloMonster>(eLayerType::Monster, this);
	Transform* diabloTr = diablo->GetComponent<Transform>();
	diabloTr->SetPosition(Vector3(4677.f, 12680.f, 1.0f));
}
