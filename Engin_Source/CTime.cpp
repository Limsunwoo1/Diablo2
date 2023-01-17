#include "CTime.h"
#include "CApplication.h"

extern CApplication Application;

static int CallCount = 0;
static float AccDeltaTime = 0.0f;
static int fps = 0;

Time::Time()
{

}

Time::~Time()
{

}

void Time::Initialize()
{
	// CPU �ʴ� �ݺ��Ǵ� �������� ������ �Լ�
	QueryPerformanceFrequency(&mCpuFrequency);

	// ���α׷� ���� ������ cpu�� Ŭ�� ��
	QueryPerformanceCounter(&mPrevFrequency);
}

void Time::Update()
{
	QueryPerformanceCounter(&mCurFrequency);

	float differenceFrequendy
		= (float)(mCurFrequency.QuadPart - mPrevFrequency.QuadPart);
	mDeltaTime = differenceFrequendy / mCpuFrequency.QuadPart;

	// �ٽ� ������ ���� ���ο� ���� ������ ����
	mPrevFrequency.QuadPart = mCurFrequency.QuadPart;

	++CallCount;
	AccDeltaTime += mDeltaTime;
}

void Time::Render(HDC hdc)
{
	wchar_t szFloat[50] = {};
	// ��Ÿ Ÿ��
	// �������� ���� ���� �� �ɸ��ð�

	/*int fps = 1.0f / mDeltaTime;

	swprintf_s(szFloat, 50, L"fps : %d", fps);
	int strLen = wcsnlen_s(szFloat, 50);

	TextOut(hdc, 10, 10, szFloat, strLen);*/
	
	if (AccDeltaTime > 1.f)
	{
		fps = CallCount;
		CallCount = 0;
		AccDeltaTime -= 1.f;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d, DT : %lf", fps, mDeltaTime);
		SetWindowText(Application.GetHwnd() , szBuffer);
	}
}

