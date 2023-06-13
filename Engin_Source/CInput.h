#pragma once
#include "CEngine.h"
#include "Math.h"
#include "..//Dx11_Engine/GuiEditor.h"

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

	// f
	F_1, F_2,F_3,F_4,F_5,F_6,
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
	Math::Vector2 GetMouseWorldPos(bool camType = true);
	Math::Vector2 GetMouseWorldPos(Vector2 windowMouse);
	Math::Vector2 GetMouseWorldPos(class Camera* camera);
	Math::Vector2 GetMouseWorldPos(class Camera* camera, Vector2 mousePos);

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

	bool GetMouseItemPick() { return mbMouseItemPick; }
	void SetMouseItemPick(bool able) { mbMouseItemPick = able; }

	class ItemBase* GetPickItem() { return mPickItem; }
	void SetPickItem(ItemBase* item) { mPickItem = item; }

	int GetIsometricX() { return mIsometricTileIndexX; }
	int GetIsometricY() { return mIsometricTileIndexY; }

	void SetIsometricX(int x) { mIsometricTileIndexX = x; }
	void SetIsometricY(int y) { mIsometricTileIndexY = y; }

	std::pair<int, int> GetIsoMetricIDX(Vector2 MousePos);
private:
	std::vector<Key> mKeys;
	Math::Vector2 mMousePos;
	std::stack<Vector2> mPosData;

	ItemBase* mPickItem;
	bool mbMouseItemPick;

	int mIsometricTileIndexX;
	int mIsometricTileIndexY;
};
