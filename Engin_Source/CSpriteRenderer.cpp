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
	if (GetRenderStop())
		return;

	if (GetOwner()->GetState() != GameObject::active)
		return;

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	GetMesh().lock()->BindBuffer();
	GetMaterial().lock()->Bind();

	Animator* animator = GetOwner()->GetComponent<Animator>();
	if (animator != nullptr)
		animator->Binds();

	GetMesh().lock()->Render();
	GetMaterial().lock()->Clear();

	if (animator != nullptr)
		animator->Clear();
}
