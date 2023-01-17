#pragma once
#include "CEngine.h"



	class Time
	{
		SINGLE(Time);

	public:
		void Initialize();
		void Update();
		void Render(HDC hdc);

		float DeltaTime() { return mDeltaTime; }

	private:
		// CPU 고유 진동수 ( 1초당 명번) 1초 12000번 진동하는CPU
		LARGE_INTEGER mCpuFrequency;
		LARGE_INTEGER mPrevFrequency;
		LARGE_INTEGER mCurFrequency;
		float mDeltaTime;
	};


