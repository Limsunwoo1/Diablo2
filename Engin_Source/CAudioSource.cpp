#include "CAudioSource.h"
#include "CAudioClip.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CTime.h"


AudioSource::AudioSource()
	: Component(eComponentType::AudioSource)
	, mAudioClip{}
{

}

AudioSource::~AudioSource()
{

}

void AudioSource::Initalize()
{
}

void AudioSource::Update()
{
	mDelta += Time::GetInstance()->DeltaTime();

	if (mAudioClip.lock() == nullptr)
		return;

	if (!mAudioClip.lock()->GetLoop() && mInterverTime > 0.0f)
	{
		if (mDelta < mInterverTime)
			return;

		mDelta = 0.0f;
		Play();
	}
}

void AudioSource::FixedUpdate()
{
	if (mAudioClip.lock() == nullptr)
		return;


	Transform* tr = GetOwner()->GetComponent<Transform>();
	Vector3 pos = tr->GetPosition();
	Vector3 foward = tr->Foward();

	mAudioClip.lock()->Set3DAttributes(pos, foward);
}

void AudioSource::Render()
{
}

void AudioSource::Play(float volum)
{
	if (mAudioClip.lock() != nullptr)
		mAudioClip.lock()->Play(volum);
}
void AudioSource::Stop()
{
	if(mAudioClip.lock() != nullptr)
		mAudioClip.lock()->Stop();
}
void AudioSource::SetLoop(bool loop, float interver)
{
	// 최초 시작시 실행시키기위해 델타 = 간격
	mInterverTime = interver;
	mDelta = interver;

	if (mAudioClip.lock() != nullptr)
		mAudioClip.lock()->SetLoop(loop);
}

void AudioSource::Clear()
{
	mAudioClip = {};
}
