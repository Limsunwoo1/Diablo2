#pragma once
#include "CEngine.h"
#include "Graphics.h"
#include "CGraphicDevice_DX11.h"

class CApplication
{
public:
	CApplication();
	~CApplication();

	virtual void Initalize();
	// CPU Update
	virtual void Update();
	virtual void Instantiate();
	virtual void Destroy();
	// GPU Update
	virtual void FixedUpdate();
	virtual void Render();

	void ThreadInit();
	void LoadScene();

	void LateUpdate();
	
	// Running main engine loop
	void Run();
	void Release();
	void Present();

	void SetWindow(HWND hwnd, UINT width, UINT height);

	void SetHwnd(HWND hwnd) { mHwnd = hwnd; }
	HWND GetHwnd() { return mHwnd; }
	UINT GetWidth() { return mWidth; }
	UINT GetHeight() { return mHeight; }

	void ComplateInitialize() { mbGameStart = true; }
	bool GetStatus() { return mbGameStart; }

private:
	bool initalized = false;
	std::unique_ptr<graphics::CGraphicDevice_DX11> graphicDevice;

	HWND mHwnd; // �ڵ鰪 - �����쿡�� �Ҵ�� �޸𸮿� �����Ҽ��ִ� ������ �ش�.
	HDC mHdc;
	UINT mHeight;
	UINT mWidth;

	bool mbGameStart;
};