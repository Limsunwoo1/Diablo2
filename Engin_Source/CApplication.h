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

	// ���� ���� ĳ���� �̵� ���
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

	HWND mHwnd; // �ڵ鰪 - �����쿡�� �Ҵ�� �޸𸮿� �����Ҽ��ִ� ������ �ش�.
	HDC mHdc;
	UINT mHeight;
	UINT mWidth;
};