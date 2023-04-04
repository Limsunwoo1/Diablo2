#pragma once
#include "CEngine.h"
#include "Math.h"

enum class eKeyCode
{
	//Alphabet
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,

	//Special Key
	ENTER, ESC, LSHIFT, LALT, LCTRL,
	SPACE, LEFT, RIGHT, UP, DOWN,
	LBTN, RBTN,

	//Num Pad
	NUM_0, NUM_1, NUM_2,
	NUM_3, NUM_4, NUM_5,
	NUM_6, NUM_7, NUM_8,
	NUM_9,

	//Num Line
	N_0, N_1, N_2, N_3, N_4, N_5, N_6, N_7, N_8, N_9,

	End,
};

enum class eKeyState
{
	DOWN,
	PRESSED,
	UP,
	NONE,
};

class Input
{
	SINGLE(Input);

public:
	struct Key
	{
		// 나의 키 종류
		eKeyCode keyCode;
		// 키보드 상태
		eKeyState state;
		bool bPressed;
	};

public:
	void Initialize();
	void Update();
	void Render(HDC hdc);
	eKeyState GetkeyState(eKeyCode keycode);

	Math::Vector2 GetMousePos() { return mMousePos; }
	Math::Vector2 GetMousePos(HWND hWnd);
	Math::Vector2 GetMouseWorldPos();
	Math::Vector2 GetMouseScreenIndex();

	__forceinline bool GetKeyPress(eKeyCode key)
	{
		return mKeys[static_cast<UINT>(key)].state == eKeyState::PRESSED;
	}

	__forceinline bool GetKeyDown(eKeyCode key)
	{
		return mKeys[static_cast<UINT>(key)].state == eKeyState::DOWN;
	}

	__forceinline bool GetKeyUp(eKeyCode key)
	{
		return mKeys[static_cast<UINT>(key)].state == eKeyState::UP;
	}
private:
	std::vector<Key> mKeys;
	Math::Vector2 mMousePos;
};


