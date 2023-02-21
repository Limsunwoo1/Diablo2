#include "CTransform.h"
#include "CRenderer.h"
#include "CInput.h"
#include "CCamera.h"

Transform::Transform()
	: Component(eComponentType::Transform)
	, mForward(Vector3::Forward)
	, mRight(Vector3::Right)
	, mUp(Vector3::Up)
	, mScale(Vector3::One)
	, mRotation(Vector3::Zero)
	, mPosition(Vector3::One)
	, mParent(nullptr)
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

	// 실제 로직상 캐릭터이동 처리
}

void Transform::FixedUpdate()
{
	// 렌더링에 사용될 위치값들을 업데이트

	// 월드 행렬 생성

	// 크기 변환 행렬
	Matrix scale = Matrix::CreateScale(mScale);

	// 회전 변환 행렬
	Matrix rotation;
	rotation = Matrix::CreateRotationX(mRotation.x);
	rotation *= Matrix::CreateRotationY(mRotation.y);
	rotation *= Matrix::CreateRotationZ(mRotation.z);

	// 이동 변환행렬
	Matrix position;
	position.Translation(mPosition);
	
	mWorld = scale * rotation * position;

	mForward = Vector3::TransformNormal(Vector3::Forward, rotation);
	mRight = Vector3::TransformNormal(Vector3::Right, rotation);
	mUp = Vector3::TransformNormal(Vector3::Up, rotation);

	// 카메라 컴포넌트에서 세팅
	// 뷰행렬, 프로젝션 행렬(투영)

	if (mParent)
		mWorld *= mParent->mWorld;

	//SetConstantBuffer();
}

void Transform::Render()
{

}

void Transform::SetConstantBuffer()
{
	Renderer::TransformCB trCb = {};
	trCb.world = mWorld;
	trCb.view = Camera::GetViewMatrix();
	trCb.projection = Camera::GetProejctionMatrix();
	// 

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Transform];
	cb->Bind(&trCb);
	cb->SetPipline(eShaderStage::VS);
}