#include "CAudioClip.h"
#include "CTransform.h"

AudioClip::AudioClip()
	: Resource(eResourceType::AudioClip)
	, mSound(nullptr)
	, mChannel(nullptr)
	, mMinDistance(1.0f)
	, mMaxDistance(1000.0f)
	, mbLoop(false)
{
	
}

AudioClip::~AudioClip()
{
	mSound->release();
	mSound = nullptr;
}

HRESULT AudioClip::Load(const std::wstring& name)
{
	std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
	std::wstring fullPath = parentPath.wstring() + L"\\Resource\\" + name;

	std::string cPath(fullPath.begin(), fullPath.end());
	if (!Fmod::GetInstance()->CreateSound(cPath, &mSound))
		return S_FALSE;

	mSound->set3DMinMaxDistance(mMinDistance, mMaxDistance);

	return S_OK;
}

void AudioClip::Play(float volume)
{
	if (mbLoop)
		mSound->setMode(FMOD_LOOP_NORMAL);
	else
		mSound->setMode(FMOD_LOOP_OFF);


	Fmod::GetInstance()->SoundPlay(mSound, &mChannel);
	mChannel->setVolume(volume);
}

void AudioClip::Stop()
{
	mChannel->stop();
}

void AudioClip::Set3DAttributes(const Vector3 pos, const Vector3 vel)
{
	FMOD_VECTOR fmodPos(pos.x, pos.y, pos.z);
	FMOD_VECTOR fmodVel(vel.x, vel.y, vel.z);

	mChannel->set3DAttributes(&fmodPos, &fmodVel);
}

void AudioClip::SetVolume(float vulume)
{
	//0 ~ 1
	mChannel->setVolume(vulume);
}
