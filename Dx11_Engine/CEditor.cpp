#include "CEditor.h"

#include "..//Engin_Source/CResourceManager.h"
#include "..//Engin_Source/CMesh.h"
#include "..//Engin_Source/CMaterial.h"
#include "..//Engin_Source/CTransform.h"
#include "..//Engin_Source/CMeshRenderer.h"

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
}

void Editor::Run()
{
	Update();
	FixedUpdate();
	Render();
}

void Editor::Update()
{
}

void Editor::FixedUpdate()
{
}

void Editor::Render()
{
}

void Editor::Release()
{
}

void Editor::DebugRender(graphics::DebugMesh& mesh)
{

}
