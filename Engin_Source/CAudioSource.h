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

	void Play();
	void Stop();
	void SetLoop(bool loop);

	void SetClip(std::weak_ptr<AudioClip> clip) { mAudioClip = clip; }
	std::weak_ptr<AudioClip> GetClip() { return mAudioClip; }

private:
	std::weak_ptr<AudioClip> mAudioClip;
};
