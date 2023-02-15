#include "CSpriteRenderer.h"
#include "CGameObject.h"
#include "CTransform.h"

SpriteRenderer::SpriteRenderer()
	: Component(eComponentType::SpriteRenderer)
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

	mMaterial->Bind();
	mMesh->BindBuffer();

	mMesh->Render();

	mMaterial->Clear();
}
