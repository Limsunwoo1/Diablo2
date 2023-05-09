#include "CCharacterSelect.h"
#include "CRenderer.h"
#include "CMeshRenderer.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CTexture2D.h"
#include "CPlayerScript.h"
#include "CCameraScript.h"
#include "CGridScript.h"
#include "CObject.h"
#include "CInput.h"
#include "CCollider2D.h"
#include "CCollisionManager.h"
#include "CBackGround.h"
#include "CSelectButtonSystem.h"
#include "CLight.h"
#include "CUIManager.h"

CharacterSelectScene::CharacterSelectScene()
	: Scene(eSceneType::Selecte)
{

}

CharacterSelectScene::~CharacterSelectScene()
{

}

void CharacterSelectScene::Initalize()
{
	{
		GameObject* directionalLight = Object::Instantiate<GameObject>(eLayerType::None, this);
		Transform* tr = directionalLight->GetComponent<Transform>();
		tr->SetPosition(Vector3(0.0f, 0.0f, -100.f));
		Light* lightcomp = directionalLight->AddComponent<Light>();
		lightcomp->SetType(eLightType::Directional);
		lightcomp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// Main Camera Game Object
	GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
	Camera* cameraComp = cameraObj->AddComponent<Camera>();
	//cameraComp->RegisterCameraInRenderer();
	cameraComp->TurnLayerMask(eLayerType::UI, false);
	cameraObj->AddComponent<CameraScript>();
	cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
	Renderer::mainCamera = cameraComp;

	{ // UiCamera
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* cameraComp = cameraObj->AddComponent<Camera>();
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->EnableLayerMasks();
		cameraComp->DisableLayerMasks();
		cameraComp->TurnLayerMask(eLayerType::UI, true);
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);

		Renderer::UiCamera = cameraComp;
	}

	// BackGround2
	{
		BackGround* ground = Object::Instantiate<BackGround>(eLayerType::BackGround, this);
		ground->SetName(L"BackGround4");

		Transform* tr = ground->GetComponent<Transform>();
		tr->SetScale(Vector3(8.0f, 6.0f, 0.0f));

		std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"SelcetBack", L"UI//characterselectscreenEXP.png");
		ground->SetTexture(tex);

		MeshRenderer* mr = ground->AddComponent<MeshRenderer>();

		std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"FadeMesh");
		std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		mr->SetMesh(mesh);
		mr->SetMaterial(material);

		//ground->Paused();
	}

	{
		SelectButtonSystem* button = Object::Instantiate<SelectButtonSystem>(eLayerType::UI, this);
		button->SetLayerType(eLayerType::UI);
		button->Active();
		button->SetName(L"Button");

		UIManager::GetInstance()->Push(L"SelectButtonSystem", button);
	}
}

void CharacterSelectScene::Update()
{
	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		//SceneManager::GetInstance()->LoadScene(eSceneType::Create);
	}

	Scene::Update();
}

void CharacterSelectScene::FixedUpdate()
{
	Scene::FixedUpdate();
}

void CharacterSelectScene::Render()
{
	Scene::Render();
}

void CharacterSelectScene::OnEnter()
{

}

void CharacterSelectScene::OnExit()
{

}