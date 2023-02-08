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
	// 상수버퍼를 가져와서 해당 상수버퍼에
	// SetConstantBuffer(eShaderStage::VS, eCBType::Transform, Renderer::triangleConstantBuffer.Get());
	// 예시처럼 값을 세팅해주어야 한다.
	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Transform];

	Vector4 pos(mPosition.x, mPosition.y, mPosition.z, 0.0f);
	cb->Bind(&pos);
	cb->SetPipline(eShaderStage::VS);
}