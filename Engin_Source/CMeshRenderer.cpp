#include "CMeshRenderer.h"
#include "CGameObject.h"
#include "CTransform.h"


MeshRenderer::MeshRenderer()
	: Component(eComponentType::MeshRenderer)
	, mMesh(nullptr)
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

	mMaterial->Bind();
	mMesh->BindBuffer();

	mMesh->Render();

	mMaterial->Clear();
}
