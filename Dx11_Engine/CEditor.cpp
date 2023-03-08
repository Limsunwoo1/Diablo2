#include "CEditor.h"

#include "..//Engin_Source/CResourceManager.h"
#include "..//Engin_Source/CMesh.h"
#include "..//Engin_Source/CMaterial.h"
#include "..//Engin_Source/CMeshRenderer.h"
#include "..//Engin_Source/CGridScript.h"
#include "..//Engin_Source/CObject.h"

Editor::Editor()
{
}

Editor::~Editor()
{
}

void Editor::Initalize()
{
	// 충돌체의 종류 갯수만큼만 있으면 된다
	mDebugObjects.resize((UINT)eColliderType::End);

	// Rect
	std::shared_ptr<Mesh> rectMesh = ResourceManager::GetInstance()->Find<Mesh>(L"DebugRectMesh");
	std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"DebugMaterial");

	mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
	MeshRenderer* meshRenderer = mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<MeshRenderer>();
	meshRenderer->SetMesh(rectMesh);
	meshRenderer->SetMaterial(material);

	// Circle
	std::shared_ptr<Mesh> circleMesh = ResourceManager::GetInstance()->Find<Mesh>(L"CircleMesh");

	mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();

	MeshRenderer* circleRenderer = mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<MeshRenderer>();
	circleRenderer->SetMesh(circleMesh);
	circleRenderer->SetMaterial(material);
	

	// Grid
	EditorObject* girdObject = new EditorObject();
	MeshRenderer* gridMr = girdObject->AddComponent<MeshRenderer>();
	gridMr->SetMesh(ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh"));
	gridMr->SetMaterial(ResourceManager::GetInstance()->Find<Material>(L"GridMaterial"));
	GridScript* gridScript = girdObject->AddComponent<GridScript>();
	gridScript->SetCamera(Renderer::mainCamera);

	mEditorObjects.push_back(girdObject);
}

void Editor::Run()
{
	Update();
	FixedUpdate();
	Render();
}

void Editor::Update()
{
	for (EditorObject* obj : mEditorObjects)
	{
		if (!obj)
			continue;

		obj->Update();
	}
}

void Editor::FixedUpdate()
{
	for (EditorObject* obj : mEditorObjects)
	{
		if (!obj)
			continue;

		obj->FixedUpdate();
	}
}

void Editor::Render()
{
	for (EditorObject* obj : mEditorObjects)
	{
		if (!obj)
			continue;

		obj->Render();
	}

	for (DebugMesh& mesh : Renderer::debugMeshes)
	{

		DebugRender(mesh);
	}

	Renderer::debugMeshes.clear();
}

void Editor::Release()
{
	for (auto* obj : mWidgets)
	{
		if (!obj)
			continue;

		delete obj;
		obj = nullptr;
	}

	for (auto* obj : mEditorObjects)
	{
		if (!obj)
			continue;

		delete obj;
		obj = nullptr;
	}

	delete mDebugObjects[(UINT)eColliderType::Rect];
	delete mDebugObjects[(UINT)eColliderType::Circle];
}

void Editor::DebugRender(graphics::DebugMesh& mesh)
{
	DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];

	Transform* tr = debugObj->GetComponent<Transform>();
	tr->SetPosition(mesh.position);
	tr->SetRotation(mesh.rotation);

	// 레이어별 차이
	if (mesh.type == eColliderType::Rect)
		tr->SetScale(mesh.scale);
	else if(mesh.type == eColliderType::Circle)
		tr->SetScale(mesh.scale);


	BaseRenderer* renderer = debugObj->GetComponent<BaseRenderer>();

	tr->FixedUpdate();

	Camera* camera = Renderer::mainCamera;
	GpuCameraMatrix(camera);

	debugObj->Render();
}

void Editor::GpuCameraMatrix(Camera* camera)
{
	Camera::SetGpuViewMatrix(camera->GetViewMatrix());
	Camera::SetGpuProjectionMatrix(camera->GetProjectionMatrix());
}
