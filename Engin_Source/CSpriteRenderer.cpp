#include "CSpriteRenderer.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"

SpriteRenderer::SpriteRenderer()
	: BaseRenderer(eComponentType::SpriteRenderer)
{
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Initalize()
{
}

void SpriteRenderer::Update()
{
}

void SpriteRenderer::FixedUpdate()
{
}

void SpriteRenderer::Render()
{
	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	GetMaterial()->Bind();
	GetMesh()->BindBuffer();

	GetMesh()->Render();

	GetMaterial()->Clear();
}
