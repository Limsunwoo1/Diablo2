#include "CSpriteRenderer.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CAnimator.h"

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

	Animator* animator = GetOwner()->GetComponent<Animator>();
	if (animator != nullptr)
		animator->Binds();

	GetMesh()->Render();
	GetMaterial()->Clear();

	if (animator != nullptr)
		animator->Clear();
}
