#include "CMeshRenderer.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"


MeshRenderer::MeshRenderer()
	: BaseRenderer(eComponentType::MeshRenderer)
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

	GetMaterial()->Bind();
	GetMesh()->BindBuffer();

	GetMesh()->Render();

	GetMaterial()->Clear();
}
