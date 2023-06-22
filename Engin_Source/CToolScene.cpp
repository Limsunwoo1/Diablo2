#include "CToolScene.h"
#include "CTilePallet.h"
#include "CCamera.h"
#include "CCameraScript.h"
#include "CObject.h"
#include "CRenderer.h"
#include "CMaterial.h"

#include "CTileObject.h"
#include "CLight.h"

#include "CInput.h"
#include "CWallObject.h"
#include "CResourceManager.h"
#include "CSceneManager.h"

#include "..//Dx11_Engine/GuiEditor.h"

extern gui::Editor _Editor;

ToolScene::ToolScene()
	: Scene(eSceneType::Tool)
	, mTilePallet(nullptr)
	, mode(eToolRenderMode::ALL)
{
}

ToolScene::~ToolScene()
{
}

void ToolScene::Initalize()
{

	{
		GameObject* directionalLight = Object::Instantiate<GameObject>(eLayerType::None, this);
		Transform* tr = directionalLight->GetComponent<Transform>();
		tr->SetPosition(Vector3(0.0f, 0.0f, -100.f));
		Light* lightcomp = directionalLight->AddComponent<Light>();
		lightcomp->SetType(eLightType::Directional);
		lightcomp->SetDiffuse(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// mainCamera
	{
		GameObject* cameraObj = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* camera = cameraObj->AddComponent<Camera>();
		camera->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent<CameraScript>();

		cameraObj->GetComponent<Transform>()->SetPosition(Vector3(5500.0f, 5500.0f, 1.0f));
		cameraObj->SetName(L"ToolMainCamera");

		camera->SetProjectionType(Camera::eProjectionType::Orthographic);

		mMainCamera = camera;
	}

	// Ui Camera
	{
		GameObject* uiCamera = Object::Instantiate<GameObject>(eLayerType::Camera, this);
		Camera* uiCameraComp = uiCamera->AddComponent<Camera>();
		uiCameraComp->DisableLayerMasks();
		uiCameraComp->TurnLayerMask(eLayerType::UI, true);

		uiCameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		mUiCamera = uiCameraComp;
	}

	{
		mTilePallet = Object::Instantiate<TilePallet>(eLayerType::TilePllet, this);
		mTilePallet->SetToolScene(this);
	}
}

void ToolScene::Update()
{
	if (mTilePallet == nullptr)
		return;

	if (Input::GetInstance()->GetKeyDown(eKeyCode::F_1))
	{
		mTilePallet->Save();
		return;
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::F_2))
	{
		mTilePallet->Load();
		return;
	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::ESC))
	{
		SceneManager::GetInstance()->LoadScene(eSceneType::MainTitle);
	}

	if (Input::GetInstance()->GetKeyDown(eKeyCode::UP))
	{
		int modenum = (int)mode + 1;
		if (modenum >= (UINT)eToolRenderMode::END)
		{
			mode = eToolRenderMode::ALL;
		}
		else
		{
			mode = (eToolRenderMode)modenum;
		}

	}
	else if (Input::GetInstance()->GetKeyDown(eKeyCode::DOWN))
	{
		int modenum = (int)mode - 1;
		if (modenum < (int)eToolRenderMode::ALL)
		{
			mode = eToolRenderMode::Unmovable_Area;
		}
		else
		{
			mode = (eToolRenderMode)modenum;
		}
	}


	switch (mode)
	{
	case eToolRenderMode::ALL:
	{
		const vector<GameObject*>& objects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : objects)
		{
			WallObject* wall = dynamic_cast<WallObject*>(obj);
			if (wall == nullptr)
				continue;

			wall->SetAlpha(1.0f);
		}

		break;
	}
	case eToolRenderMode::TILE:
	{
		const vector<GameObject*>& objects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : objects)
		{
			WallObject* wall = dynamic_cast<WallObject*>(obj);
			if (wall == nullptr)
				continue;

			wall->SetAlpha(0.6f);
		}
		break;
	}
	case eToolRenderMode::OBJECT:
	{
		const vector<GameObject*>& objects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : objects)
		{
			WallObject* wall = dynamic_cast<WallObject*>(obj);
			if (wall == nullptr)
				continue;

			wall->SetAlpha(1.0f);
		}

		break;
	}
	case eToolRenderMode::Unmovable_Area:
	{
		const vector<GameObject*>& objects = mLayers[(UINT)eLayerType::Wall].GetGameObjects();
		for (GameObject* obj : objects)
		{
			WallObject* wall = dynamic_cast<WallObject*>(obj);
			if (wall == nullptr)
				continue;

			wall->SetAlpha(0.6f);
		}
		break;
	}
	case eToolRenderMode::END:
	{
		mode = eToolRenderMode::ALL;
	}
	break;
	default:
		break;
	}

	for (Layer& layer : mLayers)
	{
		if (mode == eToolRenderMode::TILE || mode == eToolRenderMode::Unmovable_Area)
		{
			if (layer.GetLayerType() == eLayerType::Wall)
				continue;
		}

		layer.Update();
	}
}

void ToolScene::FixedUpdate()
{
	if (mTilePallet == nullptr)
		return;

	Scene::FixedUpdate();
}

void ToolScene::Render()
{
	if (mTilePallet == nullptr)
		return;

	Scene::Render();
}

void ToolScene::OnEnter()
{
	Renderer::mainCamera = GetMainCam();
	Renderer::UiCamera = GetUiCam();

	_Editor.SetWidgetsPused(false);
	_Editor.SetActive(false);
}

void ToolScene::OnExit()
{
	_Editor.SetWidgetsPused(true);
	_Editor.SetActive(true);
}
