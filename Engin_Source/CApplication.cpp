#include "CApplication.h"
#include "CRenderer.h"
#include "CTime.h"
#include "CInput.h"
#include "CSceneManager.h"
#include "CResourceManager.h"

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


void CApplication::Initalize()
{
	Time::GetInstance()->Initialize();
	Input::GetInstance()->Initialize();

	Renderer::Initialize();
	SceneManager::GetInstance()->Initalize();
}

// 게임 로직 캐릭터 이동 등등 
// CPU UPDATE
void CApplication::Update()
{
	Time::GetInstance()->Update();
	Input::GetInstance()->Update();

	SceneManager::GetInstance()->Update();
}

// GPU update
void CApplication::FixedUpdate()
{
	SceneManager::GetInstance()->FixedUpdate();
}

void CApplication::Render()
{
	Time::GetInstance()->Render(mHdc);

	graphicDevice->Clear();			  // 화면지우기
	graphicDevice->AdjustViewPorts(); // 뷰보트 크기

	SceneManager::GetInstance()->Render();

	graphicDevice->Present();		  // 스왑 체인
}
// Running main engine loop
void CApplication::Run()
{
	Update();
	FixedUpdate();
	Render();
}

void CApplication::Release()
{
	//ResourceManager::GetInstance()->Release();


	Time::GetInstance()->DestroyInstance();
	Input::GetInstance()->DestroyInstance();
	ResourceManager::GetInstance()->DestroyInstance();
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
