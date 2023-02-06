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
	if (eKeyState::DOWN == Input::GetInstance()->GetkeyState(eKeyCode::DOWN))
	{
		mPosition.y -= 0.1f;

		GetDevice()->BindsSamplers((UINT)eSamplerType::Point
			, 1, Renderer::samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
	}

	if (eKeyState::DOWN == Input::GetInstance()->GetkeyState(eKeyCode::UP))
	{
		mPosition.y += 0.1f;

		GetDevice()->BindsSamplers((UINT)eSamplerType::Anisotropic
			, 1, Renderer::samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
	}

	if (eKeyState::DOWN == Input::GetInstance()->GetkeyState(eKeyCode::LEFT))
	{
		mPosition.x -= 0.1f;
	}

	if (eKeyState::DOWN == Input::GetInstance()->GetkeyState(eKeyCode::RIGHT))
	{
		mPosition.x += 0.1f;
	}
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