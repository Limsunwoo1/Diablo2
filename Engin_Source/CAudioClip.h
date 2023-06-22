#pragma once
#include "CResource.h"
#include "CFmodManager.h"

using namespace Math;
class AudioClip : public Resource
{
public:
	AudioClip();
	~AudioClip();

	virtual HRESULT Load(const std::wstring& path) override;

	void Play();
	void Stop();
	void Set3DAttributes(const Vector3 pos, const Vector3 vel);
	void SetLoop(bool loop) { mbLoop = loop; }

private:
	FMOD::Sound* mSound;
	FMOD::Channel* mChannel;
	float mMinDistance;
	float mMaxDistance;
	bool mbLoop;
};