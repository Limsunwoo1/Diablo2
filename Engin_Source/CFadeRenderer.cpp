#include "CFadeRenderer.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CInput.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"
#include "CTime.h"

#include "iostream"
FadeRenderer::FadeRenderer()
	: BaseRenderer(eComponentType::FadeRenderer)
	, bFade(false)
	, bRun(false)
	, mEndTime(2.0f)
	, mAccTime(0.0f)
	, mStartValue(0.0f)
	, mEndValue(0.0f)
	, mCurValue(0.0f)
{

}
FadeRenderer::~FadeRenderer()
{

}

void FadeRenderer::Initalize()
{

}

void FadeRenderer::Update()
{
	if (mAccTime > mEndTime)
		Stop();

	if (Input::GetInstance()->GetkeyState(eKeyCode::SPACE) == eKeyState::PRESSED)
	{
		if (!bRun)
			InitValue(bFade);
	}

	if (!bRun)
		return;

	mAccTime += Time::GetInstance()->DeltaTime();
	mCurValue = Lerp(mStartValue, mEndValue);

	BindConstantBuffer();
}

void FadeRenderer::FixedUpdate()
{

}

void FadeRenderer::Render()
{
	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	GetMaterial().lock()->Bind();
	GetMesh().lock()->BindBuffer();

	GetMesh().lock()->Render();

	GetMaterial().lock()->Clear();
}

void FadeRenderer::BindConstantBuffer()
{
	ConstantBuffer* CB = Renderer::constantBuffers[(UINT)eCBType::Fade];
	Renderer::FadeCB data = {};

	data.alpha = mCurValue;
	CB->SetData(&data);
	CB->Bind(eShaderStage::VS);
	CB->Bind(eShaderStage::PS);
}

void FadeRenderer::InitValue(bool fade)
{
	if (!bRun && !fade)		// Fade In
	{
		mStartValue = 0.0f;
		mEndValue = 1.0f;
	}
	else if (!bRun && fade)	// Fade Out
	{
		mStartValue = 1.0f;
		mEndValue = 0.0f;
	}

	bRun = true;
}

float FadeRenderer::Lerp(float startValue, float endValue)
{
	return (startValue * (mEndTime - mAccTime) + (endValue * mAccTime)) / mEndTime;
}

void FadeRenderer::Stop()
{
	if (bFade)
		bFade = false;
	else
		bFade = true;

	bRun = false;

	mAccTime = 0.0f;
	mStartValue = 0.0f;
	mEndValue = 0.0f;
}
