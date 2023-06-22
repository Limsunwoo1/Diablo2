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
	mAudioClip.lock()->Play();
}
void AudioSource::Stop()
{
	mAudioClip.lock()->Stop();
}
void AudioSource::SetLoop(bool loop)
{
	mAudioClip.lock()->SetLoop(loop);
}