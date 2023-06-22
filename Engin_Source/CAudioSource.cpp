#include "CAudioSource.h"
#include "CAudioClip.h"
#include "CTransform.h"
#include "CGameObject.h"


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

void AudioSource::Play()
{
	if (mAudioClip.lock() != nullptr)
		mAudioClip.lock()->Play();
}
void AudioSource::Stop()
{
	if(mAudioClip.lock() != nullptr)
		mAudioClip.lock()->Stop();
}
void AudioSource::SetLoop(bool loop)
{
	if (mAudioClip.lock() != nullptr)
		mAudioClip.lock()->SetLoop(loop);
}

void AudioSource::Clear()
{
	mAudioClip = {};
}
