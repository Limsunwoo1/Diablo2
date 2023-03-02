#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CApplication.h"
#include "CRenderer.h"
#include "CSceneManager.h"
#include "CMaterial.h"
#include "CBaseRenderer.h"
#include "CSceneManager.h"

extern CApplication Application;

Matrix Camera::View = Matrix::Identity;
Matrix Camera::Projection = Matrix::Identity;

Camera::Camera()
	: Component(eComponentType::Camera)
	, mType(eProjectionType::Prespective)
	, mAspectRatio(1.0f)
	, mNear(1.0f)
	, mFar(1000.0f)
	, mScale(1.0f)
{
	EnableLayerMasks();
}

Camera::~Camera()
{
}

void Camera::Initalize()
{
}

void Camera::Update()
{
}

void Camera::FixedUpdate()
{
	CreateViewMatrix();
	CreateProjectionMatrix();

	RegisterCameraInRenderer();
}

void Camera::Render()
{
	View = mView;
	Projection = mProjection;

	SortGameObjects();

	RenderOpaqu();
	RenderCutOut();
	RenderTransparent();
}

void Camera::CreateViewMatrix()
{
	Transform* transform = GetOwner()->GetComponent<Transform>();
	Vector3 camerapos = transform->GetPosition();

	// Create Translate view matrix
	mView = Matrix::Identity;
	mView *= Matrix::CreateTranslation(-camerapos);

	// 회전 정보
	Vector3 up = transform->Up();
	Vector3 right = transform->Right();
	Vector3 forward = transform->Forward();

	Matrix viewRotate;
	viewRotate._11 = right.x; viewRotate._12 = up.x; viewRotate._13 = forward.x;
	viewRotate._21 = right.y; viewRotate._22 = up.y; viewRotate._23 = forward.y;
	viewRotate._31 = right.z; viewRotate._32 = up.z; viewRotate._33 = forward.z;

	mView *= viewRotate;
}

void Camera::CreateProjectionMatrix()
{
	RECT winRect;
	GetClientRect(Application.GetHwnd(), &winRect);

	float width = winRect.right - winRect.left;
	float height = winRect.bottom - winRect.top;
	mAspectRatio = width / height;

	if (mType == eProjectionType::Prespective)
		mProjection = Matrix::CreatePerspectiveFieldOfViewLH(XM_2PI / 6.0f, mAspectRatio, mNear, mFar);
	else
		mProjection = Matrix::CreateOrthographicLH(width / 100.0f, height / 100.0f, mNear, mFar);
}

void Camera::RegisterCameraInRenderer()
{
	eSceneType type = SceneManager::GetInstance()->GetActiveScene()->GetScenType();
	Renderer::Cameras[(UINT)type].push_back(this);
}

void Camera::TurnLayerMask(eLayerType layer, bool enable)
{
	mLayerMasks.set((UINT)layer, enable);
}

void Camera::SortGameObjects()
{
	mOpaquGameObjects.clear();
	mCutOutGameObjects.clear();
	mTransparentGameObjects.clear();

	Scene* scene = SceneManager::GetInstance()->GetActiveScene();
	for (size_t i = 0; i < (UINT)eLayerType::End; ++i)
	{
		if (mLayerMasks[i] == false)
			continue;

		Layer& layer = scene->GetLayer((eLayerType)i);
		GameObjects gameObjects = layer.GetGameObjects();
		if (gameObjects.size() == 0)
			continue;

		for (GameObject* obj : gameObjects)
		{
			PushGameObjectToRenderingMode(obj);
		}
	}
}

void Camera::RenderOpaqu()
{
	for (GameObject* obj : mOpaquGameObjects)
	{
		if (obj == nullptr)
			continue;

		obj->Render();
	}
}

void Camera::RenderCutOut()
{
	for (GameObject* obj : mCutOutGameObjects)
	{
		if (obj == nullptr)
			continue;

		obj->Render();
	}
}

void Camera::RenderTransparent()
{
	for (GameObject* obj : mTransparentGameObjects)
	{
		if (obj == nullptr)
			continue;

		obj->Render();
	}
}

void Camera::PushGameObjectToRenderingMode(GameObject* gameObject)
{
	BaseRenderer* renderer = gameObject->GetComponent<BaseRenderer>();

	if (renderer == nullptr)
		return;

	std::shared_ptr<Material> material = renderer->GetMaterial();

	if (material == nullptr)
		return;

	eRenderingMode mode = material->GetRenderingMode();

	switch (mode)
	{
	case graphics::eRenderingMode::Opaque:
		mOpaquGameObjects.push_back(gameObject);
		break;
	case graphics::eRenderingMode::CutOut:
		mCutOutGameObjects.push_back(gameObject);
		break;
	case graphics::eRenderingMode::Transparent:
		mTransparentGameObjects.push_back(gameObject);
		break;
	default:
		break;
	}
}
