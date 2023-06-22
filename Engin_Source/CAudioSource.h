#pragma once
#include "CComponent.h"
#include "CAudioClip.h"

class AudioSource : public Component
{
public:
	AudioSource();
	~AudioSource();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void Play(float volum = 0.5f);
	void Stop();
	void SetLoop(bool loop, float interver = 0.0f);

	void SetClip(std::weak_ptr<AudioClip> clip) {mAudioClip = clip; mDelta= 0.0f; mInterverTime = 0.0f;}
	void Clear();
	std::weak_ptr<AudioClip> GetClip() { return mAudioClip; }

private:
	float mInterverTime;
	float mDelta;
	std::weak_ptr<AudioClip> mAudioClip;
};
