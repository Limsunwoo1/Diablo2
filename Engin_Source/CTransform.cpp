#include "CTransform.h"
#include "CRenderer.h"
#include "CInput.h"

Transform::Transform()
	: Component(eComponentType::Transform)
{

}

Transform::~Transform()
{

}

void Transform::Initalize()
{

}

void Transform::Update()
{

}

void Transform::FixedUpdate()
{
	SetConstantBuffer();
}

void Transform::Render()
{

}

void Transform::SetConstantBuffer()
{
	// ������۸� �����ͼ� �ش� ������ۿ�
	// SetConstantBuffer(eShaderStage::VS, eCBType::Transform, Renderer::triangleConstantBuffer.Get());
	// ����ó�� ���� �������־�� �Ѵ�.
	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Transform];

	Vector4 pos(mPosition.x, mPosition.y, mPosition.z, 0.0f);
	cb->Bind(&pos);
	cb->SetPipline(eShaderStage::VS);
}