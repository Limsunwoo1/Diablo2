#include "CMeshRenderer.h"
#include "CGameObject.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CAnimator.h"


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
	if (GetRenderStop())
		return;

	if (GetOwner()->GetState() != GameObject::active)
		return;

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	GetMesh()->BindBuffer();
	GetMaterial()->Bind();

	Animator* animator = GetOwner()->GetComponent<Animator>();

	if (animator)
		animator->Binds();

	GetMesh()->Render();
	// ���ε� �Ǿ��ִ� �ؽ�ó Ŭ����
	GetMaterial()->Clear();

	if (animator)
		animator->Clear();
}
