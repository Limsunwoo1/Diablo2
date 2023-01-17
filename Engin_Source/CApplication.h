#pragma once
#include "CEngine.h"
#include "Graphics.h"
#include "CGraphicDevice_DX11.h"

class CApplication
{
public:
	CApplication();
	~CApplication();

	// Running main engine loop
	void Run();

	virtual void Initalize();

	// 게임 로직 캐릭터 이동 등등
	// CPU Update
	virtual void Update();
	
	// GPU Update
	virtual void FixedUpdate();

	virtual void Render();
	
	void Release();
	void SetWindow(HWND hwnd, UINT width, UINT height);

	void SetHwnd(HWND hwnd) { mHwnd = hwnd; }
	HWND GetHwnd() { return mHwnd; }
	UINT GetWidth() { return mWidth; }
	UINT GetHeight() { return mHeight; }
private:
	bool initalized = false;
	std::unique_ptr<graphics::CGraphicDevice_DX11> graphicDevice;

	HWND mHwnd; // 핸들값 - 윈도우에서 할당된 메모리에 접근할수있는 권한을 준다.
	HDC mHdc;
	UINT mHeight;
	UINT mWidth;
};