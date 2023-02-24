#pragma once
#include "CBaseRenderer.h"

class FadeRenderer : public BaseRenderer
{
public:
	FadeRenderer();
	virtual ~FadeRenderer();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void Stop();
	void InitValue(bool fade = true);
	float Lerp(float startValue, float endValue);
	void BindConstantBuffer();
	

private:
	bool bFade;
	bool bRun;

	float mEndTime;
	float mAccTime;

	float mStartValue;
	float mEndValue;
	float mCurValue;
};