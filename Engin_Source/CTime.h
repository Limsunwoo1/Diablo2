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
		// CPU ���� ������ ( 1�ʴ� ���) 1�� 12000�� �����ϴ�CPU
		LARGE_INTEGER mCpuFrequency;
		LARGE_INTEGER mPrevFrequency;
		LARGE_INTEGER mCurFrequency;
		float mDeltaTime;
	};


