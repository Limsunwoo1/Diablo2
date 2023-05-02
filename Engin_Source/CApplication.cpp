#include "CApplication.h"
#include "CRenderer.h"
#include "CTime.h"
#include "CInput.h"
#include "CSceneManager.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CWorldManager.h"
#include "CUIManager.h"
#include "CItemManager.h"

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
	CollisionManager::GetInstance()->Initalize();
	Renderer::Initialize();
	WorldManager::GetInstance()->Initialize();
	SceneManager::GetInstance()->Initalize();
	UIManager::GetInstance()->Initialize();
	ItemManager::GetInstance()->Initalize();
}

// 게임 로직 캐릭터 이동 등등 
// CPU UPDATE
void CApplication::Update()
{
	Time::GetInstance()->Update();
	Input::GetInstance()->Update();
	CollisionManager::GetInstance()->Update();
	SceneManager::GetInstance()->Update();
	UIManager::GetInstance()->Update();
	ItemManager::GetInstance()->Update();
}

void CApplication::Instantiate()
{
	SceneManager::GetInstance()->Instantiate();
}

// GPU update
void CApplication::FixedUpdate()
{
	CollisionManager::GetInstance()->FixedUpdate();
	SceneManager::GetInstance()->FixedUpdate();
	UIManager::GetInstance()->FixedUpdate();
	ItemManager::GetInstance()->FixedUpdate();
}

void CApplication::Render()
{
	Time::GetInstance()->Render(mHdc);

	graphicDevice->Clear();			  // 화면지우기
	graphicDevice->AdjustViewPorts(); // 뷰보트 크기

	//SceneManager::GetInstance()->Render();
	Renderer::Render();
	ItemManager::GetInstance()->Render();
}

// Running main engine loop
void CApplication::Run()
{
	Update();
	FixedUpdate();
	Render();

	Destroy();
	Instantiate();
}

void CApplication::Destroy()
{
	SceneManager::GetInstance()->Destroy();
}

void CApplication::Release()
{
	//ResourceManager::GetInstance()->Release();
	SceneManager::GetInstance()->Rlease();
	ItemManager::GetInstance()->Rlease();

	ItemManager::GetInstance()->DestroyInstance();
	UIManager::GetInstance()->DestroyInstance();
	SceneManager::GetInstance()->DestroyInstance();
	WorldManager::GetInstance()->DestroyInstance();
	Renderer::Release();
	CollisionManager::GetInstance()->DestroyInstance();
	Input::GetInstance()->DestroyInstance();
	Time::GetInstance()->DestroyInstance();
}

void CApplication::Present()
{
	graphicDevice->Present();		  // 스왑 체인
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
		//graphics::GetDevice() = graphicDevice.get();
	}

	RECT rt = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(mHwnd, true);
	UpdateWindow(mHwnd);

	/*HMENU menu = {};
	LPRECT rect = {};

	GetMenuItemRect(hwnd, menu, 0, rect);

	rect;*/
}
