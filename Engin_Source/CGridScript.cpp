#include "CGridScript.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CApplication.h"
#include "CConstantBuffer.h"
#include "CRenderer.h"
#include "CCamera.h"
#include "CSceneManager.h"

extern CApplication Application;

GridScript::GridScript()
	: Script()
	, mCamera(nullptr)
{

}
GridScript::~GridScript()
{

}

void GridScript::Initalize()
{
	eSceneType type = SceneManager::GetInstance()->GetActiveScene()->GetScenType();
	mCamera = Renderer::Cameras[(UINT)type][0];
}

void GridScript::Update()
{
	if (mCamera == nullptr)
		return;

	GameObject* gameObject = mCamera->GetOwner();
	Transform* transform = gameObject->GetComponent<Transform>();

	Vector3 cameraPos = transform->GetPosition();
	Vector4 Position = Vector4(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);

	float scale = mCamera->GetScale();

	RECT winRECT;
	GetClientRect(Application.GetHwnd(), &winRECT);
	float width = winRECT.right - winRECT.left;
	float height = winRECT.bottom - winRECT.top;
	Vector2 resolution(width, height);

	// constant buffer
	ConstantBuffer* CB = Renderer::constantBuffers[(UINT)eCBType::Grid];
	Renderer::GridCB data;
	data.cameraPosition = Position;
	data.cameraScale = Vector2(scale, scale);
	data.resolution = resolution;

	CB->Bind(&data);
	CB->SetPipline(eShaderStage::VS);
	CB->SetPipline(eShaderStage::PS);
}

void GridScript::FixedUpdate()
{

}

void GridScript::Render()
{
	
}