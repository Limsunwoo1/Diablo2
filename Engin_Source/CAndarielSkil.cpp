#include "CAndarielSkil.h"
#include "CBoltBase.h"
#include "CTime.h"

AndarielSkil::AndarielSkil()
	: Skil()
	, mInterval(0.05f)
	, mDeleta(0.f)
{
	mSpecialCastSkil.resize(9);
}

AndarielSkil::~AndarielSkil()
{
}

void AndarielSkil::Initalize()
{

	for (int i = 0; i < mSpecialCastSkil.size(); ++i)
	{
		mSpecialCastSkil[i] = new BoltBase();
		mSpecialCastSkil[i]->SetRun(true);
		mSpecialCastSkil[i]->Paused();
	}

	InitAnimation();
}

void AndarielSkil::Update()
{
	Skil::Update();

	mDeleta += Time::GetInstance()->DeltaTime();

	if (mDeleta >= mInterval)
	{
		mDeleta -= mInterval;

		for (GameObject* obj : mSpecialCastSkil)
		{
			if (obj == nullptr)
				continue;

			if (obj->GetState() != eState::paused)
			{
				obj->Active();

				// 포지션이랑 앵글 설정 해야함
				// 각도 5 도
				break;
			}
		}
	}

	for (GameObject* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		obj->Update();
	}
}

void AndarielSkil::FixedUpdate()
{
	Skil::FixedUpdate();

	for (GameObject* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		obj->FixedUpdate();
	}
}

void AndarielSkil::Render()
{
	Skil::Render();

	for (GameObject* obj : mSpecialCastSkil)
	{
		if (obj == nullptr)
			continue;

		obj->Render();
	}
}

void AndarielSkil::InitAnimation()
{

}
