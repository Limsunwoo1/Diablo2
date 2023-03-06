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
	std::shared_ptr<Mesh> rectMesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"DebugMaterial");

	mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
	mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<Transform>();

	MeshRenderer* meshRenderer = mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<MeshRenderer>();
	meshRenderer->SetMesh(rectMesh);
	meshRenderer->SetMaterial(material);

	// Circle
	std::shared_ptr<Mesh> circleMesh = ResourceManager::GetInstance()->Find<Mesh>(L"CircleMesh");

	mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();
	mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<Transform>();

	MeshRenderer* circleRenderer = mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<MeshRenderer>();
	circleRenderer->SetMesh(circleMesh);
	circleRenderer->SetMaterial(material);
	

	// Grid
	EditorObject* girdObject = new EditorObject();
	MeshRenderer* gridMr = girdObject->AddComponent<MeshRenderer>();
	gridMr->SetMesh(ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh"));
	gridMr->SetMaterial(ResourceManager::GetInstance()->Find<Material>(L"DebugMaterial"));
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
	Renderer::debugMeshes.clear();

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

	/*for (DebugObject* obj : mDebugObjects)
	{
		if (!obj)
			continue;

		obj->Render();
	}

	for (Widget* obj : mWidgets)
	{
		if (!obj)
			continue;

		obj->Render();
	}*/
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

	for (auto* obj : mDebugObjects)
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
}

void Editor::DebugRender(graphics::DebugMesh& mesh)
{
	DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];

	Transform* tr = debugObj->GetComponent<Transform>();
	tr->SetPosition(mesh.position);
	tr->SetRotation(mesh.rotation);

	if (mesh.type == eColliderType::Rect)
		tr->SetScale(mesh.scale);
	else if(mesh.type == eColliderType::Circle)
		tr->SetScale(mesh.scale);

	BaseRenderer* renderer = debugObj->GetComponent<BaseRenderer>();
	Camera* camera = Renderer::mainCamera;

	tr->FixedUpdate();

	Camera::SetGpuViewMatrix(Renderer::mainCamera->GetViewMatrix());
	Camera::SetGpuProjectionMatrix(Renderer::mainCamera->GetProjectionMatrix());

	debugObj->Render();
}
