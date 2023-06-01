#include "CTime.h"
#include "CInput.h"
#include "CApplication.h"

#include "..//Dx11_Engine/GuiEditor.h"

extern CApplication Application;
extern gui::Editor _Editor;

static int CallCount = 0;
static float AccDeltaTime = 0.0f;
static int fps = 0;
static float MouseX = 0.0f;
static float MouseY = 0.0f;

Time::Time()
	: mCpuFrequency{}
	, mPrevFrequency{}
	, mCurFrequency{}
	, mDeltaTime (0.f)
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
		= static_cast<float>(mCurFrequency.QuadPart - mPrevFrequency.QuadPart);
	mDeltaTime = differenceFrequendy / static_cast<float>(mCpuFrequency.QuadPart);

	// �ٽ� ������ ���� ���ο� ���� ������ ����
	mPrevFrequency.QuadPart = mCurFrequency.QuadPart;

	++CallCount;
	AccDeltaTime += mDeltaTime;
}

void Time::Render(HDC hdc)
{
	Math::Vector2 pos = Input::GetInstance()->GetMouseWorldPos(true);

	if (_Editor.GetActive())
	{
		pos = _Editor.GetEditorWorldMousePos();
	}

	MouseX = pos.x;
	MouseY = pos.y;

	float IndexX = 0;
	float IndexY = 0;


	IndexX = (((MouseX - 5000.f) / 100) + ((MouseY - 5000.f) / 50)) / 2;
	IndexY = (((MouseY - 5000.f) / 50) - ((MouseX - 5000.f) / 100)) / 2;

	if (IndexX <= -1.0f || IndexY <= -1.0f)
	{
		IndexX -= 1.f;
		IndexY -= 1.f;
	}
	else
	{
		IndexX += 0.5f;
		IndexY += 0.5f;
	}


	Input::GetInstance()->SetIsometricX((int)IndexX);
	Input::GetInstance()->SetIsometricY((int)IndexY);

	//wchar_t szFloat[50] = {};
	// ��Ÿ Ÿ��
	// �������� ���� ���� �� �ɸ��ð�

	/*int fps = 1.0f / mDeltaTime;

	swprintf_s(szFloat, 50, L"fps : %d", fps);
	int strLen = wcsnlen_s(szFloat, 50);

	TextOut(hdc, 10, 10, szFloat, strLen);*/
	
		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d  DT : %lf   MX : %lf   MY  : %lf  IX : %d  IY  :  %d", fps, mDeltaTime, MouseX, MouseY, (int)IndexX, (int)IndexY);
	if (AccDeltaTime > 1.0f)
	{
		fps = CallCount;
		CallCount = 0;
		AccDeltaTime -= 1.0f;

		SetWindowText(Application.GetHwnd() , szBuffer);
	}
}

