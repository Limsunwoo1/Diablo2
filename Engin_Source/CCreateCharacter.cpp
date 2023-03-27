#include "CCreateCharacter.h"
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

CreateCharactorScene::CreateCharactorScene()
	: Scene(eSceneType::Create)
{

}

CreateCharactorScene::~CreateCharactorScene()
{

}

void CreateCharactorScene::Initalize()
{
	// Main Camera Game Object
	GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
	Camera* cameraComp = cameraObj->AddComponent<Camera>();
	//cameraComp->RegisterCameraInRenderer();
	cameraComp->TurnLayerMask(eLayerType::UI, false);
	cameraObj->AddComponent<CameraScript>();
	cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
	Renderer::mainCamera = cameraComp;

	//// BackGround2
	//{
	//	BackGround* ground = Object::Instantiate<BackGround>(eLayerType::BackGround, this);
	//	ground->SetName(L"BackGround5");

	//	ground->FindTextuer2D(L"TitleTexture5", L"UI\\charactercreationsCreate.png");
	//	Transform* tr = ground->GetComponent<Transform>();
	//	tr->SetScale(Vector3(8.0f, 6.0f, 0.0f));

	//	MeshRenderer* mr = ground->AddComponent<MeshRenderer>();

	//	std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"FadeMesh");
	//	std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
	//	mr->SetMesh(mesh);
	//	mr->SetMaterial(material);

	//	//ground->Paused();
	//}

	Scene::Initalize();
}

void CreateCharactorScene::Update()
{
	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		SceneManager::GetInstance()->LoadScene(eSceneType::Play);
	}

	Scene::Update();
}

void CreateCharactorScene::FixedUpdate()
{
	Scene::FixedUpdate();
}

void CreateCharactorScene::Render()
{
	Scene::Render();
}

void CreateCharactorScene::OnEnter()
{

}

void CreateCharactorScene::OnExit()
{

}