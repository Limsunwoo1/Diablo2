#include "CApplication.h"
#include "CRenderer.h"
#include "CTime.h"
#include "CInput.h"

using namespace graphics;

CApplication::CApplication()
	: mHwnd(nullptr)
	, mWidth(0)
	, mHeight(0)
	, graphicDevice(nullptr)
{
}

CApplication::~CApplication()
{
}

// Running main engine loop
void CApplication::Run()
{
	Update();
	FixedUpdate();
	Render();
}

void CApplication::Initalize()
{
	Time::GetInstance()->Initialize();
	Input::GetInstance()->Initialize();

	Renderer::Initialize();
}

// 게임 로직 캐릭터 이동 등등 
// CPU UPDATE
void CApplication::Update()
{
	Time::GetInstance()->Update();
	Input::GetInstance()->Update();
}

// GPU update
void CApplication::FixedUpdate()
{
}

void CApplication::Render()
{
	Time::GetInstance()->Render(mHdc);
	graphicDevice->Draw();
}

void CApplication::Release()
{
	Time::GetInstance()->DestroyInstance();
	Input::GetInstance()->DestroyInstance();
}

void CApplication::SetWindow(HWND hwnd, UINT width, UINT height)
{
	if (graphicDevice == nullptr)
	{
		mHwnd = hwnd;
		mHdc = GetDC(mHwnd);
		mWidth = width;
		mHeight = height;

		ValidationMode validationMode = ValidationMode::Disabled;

		// 스마트 포인터
		graphicDevice = std::make_unique<CGraphicDevice_DX11>();
		graphics::GetDevice() = graphicDevice.get();
	}

	RECT rt = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(mHwnd, true);
	UpdateWindow(mHwnd);
}
