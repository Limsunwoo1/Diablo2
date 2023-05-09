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

	Vector3 radian(mRotation.x * (XM_PI / 180),
					mRotation.y * (XM_PI / 180),
					mRotation.z * (XM_PI / 180));

	rotation = Matrix::CreateRotationX(radian.x);
	rotation *= Matrix::CreateRotationY(radian.y);
	rotation *= Matrix::CreateRotationZ(radian.z);



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
	{
		mWorld._11 *= mParent->mWorld._11;
		mWorld._22 *= mParent->mWorld._22;
		mWorld._33 *= mParent->mWorld._33;

		mWorld._41 += mParent->mWorld._41;
		mWorld._42 += mParent->mWorld._42;
		mWorld._43 += mParent->mWorld._43;

		/*Matrix mat;
		mat._11 = 4.0f; mat._12 = 0.0f; mat._13 = 0.0f; mat._14 = 0.0f;
		mat._21 = 4.0f; mat._22 = 2.0f; mat._23 = 0.0f; mat._24 = 0.0f;
		mat._31 = 0.0f; mat._32 = 0.0f; mat._33 = 0.0f; mat._34 = 0.0f;
		mat._41 = 4.0f; mat._42 = 2.0f; mat._43 = 0.0f; mat._44 = 1.0f;
		
		Matrix mat1;
		mat._11 = 1.5f; mat._12 = 0.0f; mat._13 = 0.0f; mat._14 = 0.0f;
		mat._21 = 0.0f; mat._22 = 1.0f; mat._23 = 0.0f; mat._24 = 0.0f;
		mat._31 = 0.0f; mat._32 = 0.0f; mat._33 = 0.0f; mat._34 = 0.0f;
		mat._41 = 1.0f; mat._42 = 1.0f; mat._43 = 0.0f; mat._44 = 1.0f;

		mat1 *= mat;

		mWorld *= mParent->mWorld;*/
	}
}

void Transform::Render()
{

}

void Transform::SetConstantBuffer()
{
	Renderer::TransformCB trCb = {};
	trCb.world = mWorld;
	trCb.view = Camera::GetGpuViewMatrix();
	trCb.projection = Camera::GetGpuProejctionMatrix();
	// 

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Transform];
	cb->SetData(&trCb);
	cb->Bind(eShaderStage::VS);
	cb->Bind(eShaderStage::HS);
	cb->Bind(eShaderStage::DS);
	cb->Bind(eShaderStage::GS);
	cb->Bind(eShaderStage::PS);
	cb->Bind(eShaderStage::CS);
}