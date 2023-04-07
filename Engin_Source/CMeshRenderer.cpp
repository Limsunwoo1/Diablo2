#include "CMeshRenderer.h"
#include "CGameObject.h"
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
	if (GetOwner()->GetState() != GameObject::active)
		return;

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	GetMesh()->BindBuffer();
	GetMaterial()->Bind();

	GetMesh()->Render();
	
	// ���ε� �Ǿ��ִ� �ؽ�ó Ŭ����
	GetMaterial()->Clear();
}
