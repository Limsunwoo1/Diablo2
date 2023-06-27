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
#include "FileManager.h"
#include "CObjectManager.h"
#include "CFmodManager.h"

using namespace graphics;

CApplication::CApplication()
	: mHwnd(nullptr)
	, mWidth(0)
	, mHeight(0)
	, graphicDevice(nullptr)
	, mbGameStart(false)
{
}

CApplication::~CApplication()
{

}


void CApplication::Initalize()
{
	Time::GetInstance()->Initialize();
	Input::GetInstance()->Initialize();
	ObjectManager::GetInstance()->Initialize();
	CollisionManager::GetInstance()->Initalize();
	WorldManager::GetInstance()->Initialize();

	UIManager::GetInstance()->Initialize();
	ItemManager::GetInstance()->Initalize();

	SceneManager::GetInstance()->LateInitalize();
}

// ���� ���� ĳ���� �̵� ��� 
// CPU UPDATE
void CApplication::Update()
{
	Time::GetInstance()->Update();
	Input::GetInstance()->Update();
	CollisionManager::GetInstance()->Update();
	ObjectManager::GetInstance()->Update();
	WorldManager::GetInstance()->Update();

	SceneManager::GetInstance()->Update();
	UIManager::GetInstance()->Update();
	ItemManager::GetInstance()->Update();
}

void CApplication::ThreadInit()
{
	Renderer::Initialize();
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
	graphicDevice->Clear();			  // ȭ�������
	graphicDevice->AdjustViewPorts(); // �亸Ʈ ũ��

	//SceneManager::GetInstance()->Render();
	Time::GetInstance()->Render(mHdc);
	Renderer::Render();
	ItemManager::GetInstance()->Render();
}

void CApplication::LateUpdate()
{
	Destroy();
	Instantiate();
}

// Running main engine loop
void CApplication::Run()
{
	Update();
	FixedUpdate();
	Render();

	ObjectManager::GetInstance()->LateUpdate();
}

void CApplication::Destroy()
{
	SceneManager::GetInstance()->Destroy();
}

void CApplication::Release()
{
	// �޸������� ���̺� ���� ����
	FileManager::GetInstance()->WriteSaveFile();

	// ���� ������Ʈ �޸� ����
	//ResourceManager::GetInstance()->Release();
	SceneManager::GetInstance()->Rlease();
	ItemManager::GetInstance()->Rlease();
	ObjectManager::GetInstance()->Realse();
	UIManager::GetInstance()->Rlease();

	// �̱��� �Ŵ��� �޸� ����
	FileManager::GetInstance()->DestroyInstance();
	ItemManager::GetInstance()->DestroyInstance();
	UIManager::GetInstance()->DestroyInstance();
	SceneManager::GetInstance()->DestroyInstance();
	WorldManager::GetInstance()->DestroyInstance();
	Renderer::Release();
	CollisionManager::GetInstance()->DestroyInstance();
	ObjectManager::GetInstance()->DestroyInstance();
	Input::GetInstance()->DestroyInstance();
	Time::GetInstance()->DestroyInstance();

	ResourceManager::GetInstance()->DestroyInstance();

	Fmod::GetInstance()->Release();
	Fmod::GetInstance()->DestroyInstance();
}

void CApplication::Present()
{
	graphicDevice->Present();		  // ���� ü��
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

		// ����Ʈ ������
		graphicDevice = std::make_unique<CGraphicDevice_DX11>();
		//graphics::GetDevice() = graphicDevice.get();
	}

	RECT rt = { 0, 0, (LONG)width, (LONG)height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(mHwnd, true);
	UpdateWindow(mHwnd);

	mWidth = rt.right + rt.left;
	mHeight = rt.bottom + rt.top;
	/*HMENU menu = {};
	LPRECT rect = {};

	GetMenuItemRect(hwnd, menu, 0, rect);

	rect;*/
}

void CApplication::LoadScene()
{
	std::cout << "Load..." << std::endl;

	Sleep(10.f);
}
