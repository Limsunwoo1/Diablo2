#pragma once
#include "CComponent.h"

class AudioListener : public Component
{
public:
	AudioListener();
	~AudioListener();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

private:
};