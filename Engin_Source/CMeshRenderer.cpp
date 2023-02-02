#include "CMeshRenderer.h"
#include "CGameObject.h"
#include "CTransform.h"


MeshRenderer::MeshRenderer()
	: Component(eComponentType::Mesh)
	, mMesh(nullptr)
	, mShader(nullptr)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Initalize()
{
}

void MeshRenderer::Update()
{
}

void MeshRenderer::FixedUpdate()
{
}

void MeshRenderer::Render()
{
	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	mMesh->BindBuffer();
	mShader->Binds();

	mMesh->Render();
}
