#include "CTime.h"
#include "CInput.h"
#include "CApplication.h"

#include "..//Dx11_Engine/GuiEditor.h"
#include "CSceneManager.h"
#include "CToolScene.h"

extern CApplication Application;
extern gui::Editor _Editor;

static int CallCount = 0;
static float AccDeltaTime = 0.0f;
static int fps = 0;

Time::Time()
	: mCpuFrequency{}
	, mPrevFrequency{}
	, mCurFrequency{}
	, mDeltaTime(0.f)
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

	auto idx = Input::GetInstance()->GetIsoMetricIDX(pos);

	Input::GetInstance()->SetIsometricX((int)idx.first);
	Input::GetInstance()->SetIsometricY((int)idx.second);


	//wchar_t szFloat[50] = {};
	// ��Ÿ Ÿ��
	// �������� ���� ���� �� �ɸ��ð�

	/*int fps = 1.0f / mDeltaTime;

	swprintf_s(szFloat, 50, L"fps : %d", fps);
	int strLen = wcsnlen_s(szFloat, 50);

	TextOut(hdc, 10, 10, szFloat, strLen);*/

	wchar_t szBuffer[255] = {};
	swprintf_s(szBuffer, L"FPS : %d  DT : %lf   MX : %lf   MY  : %lf  IX : %d  IY  :  %d", fps, mDeltaTime, pos.x, pos.y, (int)idx.first, (int)idx.second);
	ToolScene* tool = dynamic_cast<ToolScene*>(SceneManager::GetInstance()->GetActiveScene());
	if (tool != nullptr)
	{
		wchar_t Buffer[255] = {};

		if (tool->GetToolRenderMode() == eToolRenderMode::TILE)
		{
			swprintf_s(szBuffer, L"FPS : %d  DT : %lf   MX : %lf   MY  : %lf  IX : %d  IY  :  %d     TileMode", fps, mDeltaTime, pos.x, pos.y, (int)idx.first, (int)idx.second);
		}
		else if (tool->GetToolRenderMode() == eToolRenderMode::OBJECT)
		{
			swprintf_s(szBuffer, L"FPS : %d  DT : %lf   MX : %lf   MY  : %lf  IX : %d  IY  :  %d     ObjectMode", fps, mDeltaTime, pos.x, pos.y, (int)idx.first, (int)idx.second);
		}
		else if (tool->GetToolRenderMode() == eToolRenderMode::Unmovable_Area)
		{
			swprintf_s(szBuffer, L"FPS : %d  DT : %lf   MX : %lf   MY  : %lf  IX : %d  IY  :  %d     CarveMode", fps, mDeltaTime, pos.x, pos.y, (int)idx.first, (int)idx.second);
		}
		else
		{
			swprintf_s(szBuffer, L"FPS : %d  DT : %lf   MX : %lf   MY  : %lf  IX : %d  IY  :  %d     ALLMode", fps, mDeltaTime, pos.x, pos.y, (int)idx.first, (int)idx.second);
		}
	}

	SetWindowText(Application.GetHwnd(), szBuffer);

	if (AccDeltaTime > 1.0f)
	{
		fps = CallCount;
		CallCount = 0;
		AccDeltaTime -= 1.0f;

		//SetWindowText(Application.GetHwnd(), szBuffer);
	}
}

