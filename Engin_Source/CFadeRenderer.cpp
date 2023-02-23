#include "CFadeRenderer.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CInput.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"
#include "CTime.h"

FadeRenderer::FadeRenderer()
	: BaseRenderer(eComponentType::FadeRenderer)
	, bfade(false)
	, mDelta(0.0f)
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
	ConstantBuffer* CB = Renderer::constantBuffers[(UINT)eCBType::Fade];
	Renderer::FadeCB data = {};

	if (Input::GetInstance()->GetkeyState(eKeyCode::SPACE) == eKeyState::DOWN)
	{
		if ((!bfade) && (mDelta <= 0.0f))
		{
			bfade = true;
			mDelta = 0.0f;
		}
		else if (bfade && (mDelta >= 1.0f))
		{
			bfade = false;
			mDelta = 1.0f;
		}
	}

	if (bfade && (mDelta < 1.0f))
		mDelta += Time::GetInstance()->DeltaTime();
	if(!bfade && (mDelta > 0.0f))
		mDelta -= Time::GetInstance()->DeltaTime();

	data.delta = mDelta;
	CB->Bind(&data);
	CB->SetPipline(eShaderStage::VS);
	CB->SetPipline(eShaderStage::PS);
}

void FadeRenderer::FixedUpdate()
{

}

void FadeRenderer::Render()
{
	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	GetMaterial()->Bind();
	GetMesh()->BindBuffer();

	GetMesh()->Render();

	GetMaterial()->Clear();
}