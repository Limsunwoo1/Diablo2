// Dx11_Engine.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Dx11_Engine.h"
#include "Engin_Source//CApplication.h"
#include "GuiEditor.h"
#include <Resource.h>
#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <mutex>

#include "Engin_Source/CRenderer.h"
#include "Engin_Source/CSceneManager.h"
#include "Engin_Source/CFmodManager.h"


#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\Lib\\Engin_Source.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\Lib\\Engin_Source.lib")
#endif


#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

CApplication Application;
gui::Editor _Editor;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
bool ThreadEnd();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(2182 );

	 // 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_DX11ENGINE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	std::thread GameStart(ThreadEnd);

	// 스레드가 백그라운드에서 돈다
	GameStart.detach();

	// 해당스레드 완료까지 Lock;
	// GameStart.join();

	//thread gameOn(std::move(InitFinish), true);
	//gameOn.join();

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DX11ENGINE));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (true)
	{
		// 일반적으로는 GetMessage를 사용
		// Qeue에서 메세지를 하나씩 빼와서 사용
		// 메세지가 없을시 프로그램이 멈춘거 처럼보임
		// 게임에서는 PeekMessage를 사용
		// 메세지가 없어도 돌아간다
		// 게임에 드로우 영역 때문
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (Application.GetStatus() == false)
			{
				Application.LoadScene();
				continue;
			}

			Application.Run();
			_Editor.Run();

			Application.Present();
			Application.LateUpdate();


			// 여기서 게임 실행 로직이 돌아가야한다.

			// 라이브러리
			// 하나이상의 서브루틴이나 함수의 집합파일

			// 분업하기 편하다

			// 라이브러리 종류
			// 정적라이브러리와 동적라이브러리가 있다.

			// 동적라이브러리 : 완선된 프로그램을 실행 할때 포함시키는 라이브러리
			// 윈도우기준 -> .dll
			// 리눅스기준 -> .so
			// 장점 : 메모리를 절약 할 수 있다. 스와핑 현상 적게 일어난다.
			// 프로그램이 한번 메모리에 올려진것을 공유하므로 사용공간자체가 정적라이브러리보다 적다.
			// 프로그램 중간에 동적라이브러리를 업데이트하거나 변경해도 원본프로그램에는 문제가없다.
			// 잘못업데이트하면 중간에 프로그램이 죽어버린다.

			// 정적 라이브러리 : 프로그램을 컴파일하는과정에서 포함시키는 라이브러리
			// 윈도우기준 -> .lib
			// 리눅스기준 -> .a
			// 단점 : 동적라이브러리에 비해서 각각 할당해줘야하기 때문에 용량을 많이 차지한다.
			// 해당 정적 라이브러리를 참조하는 프로그램이 많아질수록 메모리 사용량이 배로 늘어난다.
			// 재 배포해줄시에 프로그램을 종료하고 새로 빌드한후 배포해주어야 한다.

			// 최대큰 단점! : cpp 파일이 많아져서 링킹이 엄청 오래 걸린다.

			// 장점 : 컴파일시 프로그램에 적재되기 때문에 이식성이 좋다.
			// 런타임시에 동적라이브러리 파일처럼 다른 메모리에 참조할 필요가 없기 때문에
			// 속도에 장점이 있다.


		}
	}

	_Editor.Release();
	Application.Release();
	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_MYICON));
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_MY_CURSOR1));
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr/*MAKEINTRESOURCEW(IDC_DX11ENGINE)*/;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, 1600, 900, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	Application.SetWindow(hWnd, 1600, 900);

	return TRUE;
}

bool ThreadEnd()
{
	std::mutex m;
	std::thread rendererInit([&m]() {
		m.lock();

		Renderer::Initialize();
		Fmod::GetInstance()->Initialize();
		SceneManager::GetInstance()->Initalize();
		m.unlock();

		});

	rendererInit.join();

	std::thread ApllicationInit([]()
		{
			Application.Initalize();
			Application.ComplateInitialize();
		});

	std::thread EditorInit([]()
		{
			_Editor.Initalize();
		});

	ApllicationInit.join();
	EditorInit.join();

	return true;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wPram, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			// const int dpt = HIWORD(wParam);
			 //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, nullptr,
				suggested_rect->left, suggested_rect->top,
				suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top,
				SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
