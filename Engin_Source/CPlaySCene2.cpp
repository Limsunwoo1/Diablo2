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

		AudioSource* audio = cameraObj->AddComponent<AudioSource>();
		std::weak_ptr<AudioClip>clip = ResourceManager::GetInstance()->Load<AudioClip>(L"Ac4Sound", L"SoundResource\\Act4Mesa.mp3");
		audio->SetClip(clip);
		audio->SetLoop(true);
		
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

	DiabloMonster* diablo = Object::Instantiate<DiabloMonster>(eLayerType::Monster, this);
	Transform* diabloTr = diablo->GetComponent<Transform>();
	diabloTr->SetPosition(Vector3(4677.f, 12680.f, 1.0f));

	Boss = diablo;

	{
		MinosTauros* mino0 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* mino0Tr = mino0->GetComponent<Transform>();
		mino0Tr->SetPosition(Vector3(7590.f, 8150.f, 1.0f));

		MinosTauros* mino = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr = mino->GetComponent<Transform>();
		minoTr->SetPosition(Vector3(7622.f, 7999.f, 1.0f));

		MinosTauros* mino2 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr2 = mino2->GetComponent<Transform>();
		minoTr2->SetPosition(Vector3(7966.f, 8288.f, 1.0f));

		MinosTauros* mino3 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr3 = mino3->GetComponent<Transform>();
		minoTr3->SetPosition(Vector3(7140.f, 8300.f, 1.0f));

		MinosTauros* mino4 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr4 = mino4->GetComponent<Transform>();
		minoTr4->SetPosition(Vector3(6140, 9100.f, 1.0f));
	}

	{
		MephistoMonster* mino2 = Object::Instantiate<MephistoMonster>(eLayerType::Monster, this);
		Transform* minoTr2 = mino2->GetComponent<Transform>();
		minoTr2->SetPosition(Vector3(7600.f, 8450.f, 1.0f));

		MephistoMonster* mino3 = Object::Instantiate<MephistoMonster>(eLayerType::Monster, this);
		Transform* minoTr3 = mino3->GetComponent<Transform>();
		minoTr3->SetPosition(Vector3(4786, 10654.f, 1.0f));

		MephistoMonster* mino4 = Object::Instantiate<MephistoMonster>(eLayerType::Monster, this);
		Transform* minoTr4 = mino4->GetComponent<Transform>();
		minoTr4->SetPosition(Vector3(4622, 10514.f, 1.0f));
	}

	{
		MinosTauros* mino = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr = mino->GetComponent<Transform>();
		minoTr->SetPosition(Vector3(6477.f, 8292.f, 1.0f));

		MinosTauros* mino2 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr2 = mino2->GetComponent<Transform>();
		minoTr2->SetPosition(Vector3(6477.f + 100.f, 8292.f + 100.f, 1.0f));

		MinosTauros* mino3 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr3 = mino3->GetComponent<Transform>();
		minoTr3->SetPosition(Vector3(6477.f - 100.f, 8292.f - 100.f, 1.0f));

		MinosTauros* mino4 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr4 = mino4->GetComponent<Transform>();
		minoTr4->SetPosition(Vector3(6477.f + 100.f, 8292.f - 100.f, 1.0f));

		MinosTauros* mino5 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr5 = mino5->GetComponent<Transform>();
		minoTr5->SetPosition(Vector3(6477.f - 100.f, 8292.f + 100.f, 1.0f));
	}

	{
		MinosTauros* mino = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr = mino->GetComponent<Transform>();
		minoTr->SetPosition(Vector3(7155.f, 8939.f, 1.0f));

		MinosTauros* mino2 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr2 = mino2->GetComponent<Transform>();
		minoTr2->SetPosition(Vector3(7155.f + 100.f, 8939.f + 100.f, 1.0f));

		MinosTauros* mino3 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr3 = mino3->GetComponent<Transform>();
		minoTr3->SetPosition(Vector3(7155.f - 100.f, 8939.f - 100.f, 1.0f));

		MinosTauros* mino4 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr4 = mino4->GetComponent<Transform>();
		minoTr4->SetPosition(Vector3(7155.f + 100.f, 8939.f - 100.f, 1.0f));

		MinosTauros* mino5 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr5 = mino5->GetComponent<Transform>();
		minoTr5->SetPosition(Vector3(7155.f - 100.f, 8939.f + 100.f, 1.0f));
	}

	{
		MinosTauros* mino = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr = mino->GetComponent<Transform>();
		minoTr->SetPosition(Vector3(6532.f, 9253.f, 1.0f));

		MinosTauros* mino2 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr2 = mino2->GetComponent<Transform>();
		minoTr2->SetPosition(Vector3(6532.f + 100.f, 9253.f + 100.f, 1.0f));

		MinosTauros* mino3 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr3 = mino3->GetComponent<Transform>();
		minoTr3->SetPosition(Vector3(6532.f - 100.f, 9253.f - 100.f, 1.0f));

		MinosTauros* mino4 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr4 = mino4->GetComponent<Transform>();
		minoTr4->SetPosition(Vector3(6532.f + 100.f, 9253.f - 100.f, 1.0f));

		MinosTauros* mino5 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr5 = mino5->GetComponent<Transform>();
		minoTr5->SetPosition(Vector3(6532.f - 100.f, 9253.f + 100.f, 1.0f));
	}

	{
		MinosTauros* mino = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr = mino->GetComponent<Transform>();
		minoTr->SetPosition(Vector3(5372.f, 9592.f, 1.0f));

		MinosTauros* mino2 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr2 = mino2->GetComponent<Transform>();
		minoTr2->SetPosition(Vector3(5372.f + 100.f, 9592.f + 100.f, 1.0f));

		MinosTauros* mino3 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr3 = mino3->GetComponent<Transform>();
		minoTr3->SetPosition(Vector3(5372.f - 100.f, 9592.f - 100.f, 1.0f));

		MinosTauros* mino4 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr4 = mino4->GetComponent<Transform>();
		minoTr4->SetPosition(Vector3(5372.f + 100.f, 9592.f - 100.f, 1.0f));

		MinosTauros* mino5 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr5 = mino5->GetComponent<Transform>();
		minoTr5->SetPosition(Vector3(5372.f - 100.f, 9592.f + 100.f, 1.0f));
	}

	{
		MinosTauros* mino = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr = mino->GetComponent<Transform>();
		minoTr->SetPosition(Vector3(3669.f, 9997.f, 1.0f));

		MinosTauros* mino2 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr2 = mino2->GetComponent<Transform>();
		minoTr2->SetPosition(Vector3(3669.f + 100.f +100.f, 9997.f + 100.f, 1.0f));

		MinosTauros* mino3 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr3 = mino3->GetComponent<Transform>();
		minoTr3->SetPosition(Vector3(3669.f - 100.f + 200.f, 9997.f - 100.f, 1.0f));

		MinosTauros* mino4 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr4 = mino4->GetComponent<Transform>();
		minoTr4->SetPosition(Vector3(3669.f + 100.f + 300.f, 9997.f - 100.f, 1.0f));

		MinosTauros* mino5 = Object::Instantiate<MinosTauros>(eLayerType::Monster, this);
		Transform* minoTr5 = mino5->GetComponent<Transform>();
		minoTr5->SetPosition(Vector3(3669.f - 100.f + 400.f, 9997.f + 100.f, 1.0f));
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
	UiBase* skiltree = UIManager::GetInstance()->GetUiInstance<UiBase>(L"SkilTree");
	UiBase* panel = UIManager::GetInstance()->GetUiInstance<UiBase>(L"SkilSelect");
	UiBase* panelbutton = UIManager::GetInstance()->GetUiInstance<UiBase>(L"SkilSelectButton");

	Object::Instantiate<InventoryPanel>(eLayerType::UI, invenpanel, true, eSceneType::Play2);
	Object::Instantiate<MainPanel>(eLayerType::UI, mainpanel, true, eSceneType::Play2);
	Object::Instantiate<Panel>(eLayerType::UI, monsterHp, true, eSceneType::Play2);
	Object::Instantiate<UiBase>(eLayerType::UI, skiltree, true, eSceneType::Play2);
	Object::Instantiate<UiBase>(eLayerType::UI, panel, true, eSceneType::Play2);
	Object::Instantiate<UiBase>(eLayerType::UI, panelbutton, true, eSceneType::Play2);


	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::PlayerSKil, eLayerType::Monster, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::MonsterSkil, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::PlayerSKil, eLayerType::Wall, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::MonsterSkil, eLayerType::Wall, false);
	CollisionManager::GetInstance()->CollisionlayerCheck(eLayerType::Player, eLayerType::Gate, false);

	GetMainCam()->GetOwner()->GetComponent<AudioSource>()->Play(0.3f);
}

void PlayScene2::OnExit()
{
	ItemManager::GetInstance()->ClearWorldItem();

	GetMainCam()->GetOwner()->GetComponent<AudioSource>()->Stop();
}

void PlayScene2::OnEvent()
{
	/*DiabloMonster* diablo = Object::Instantiate<DiabloMonster>(eLayerType::Monster, this);
	Transform* diabloTr = diablo->GetComponent<Transform>();
	diabloTr->SetPosition(Vector3(4677.f, 12680.f, 1.0f));*/

	if (Boss)
		Boss->Active();
}
