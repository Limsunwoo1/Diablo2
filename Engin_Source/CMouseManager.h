#pragma once
#include "CEngine.h"
#include "CInput.h"

class MouseManager
{
	SINGLE(MouseManager)
public:
	enum class MouseState
	{
		None,
		Down,
		Up,
		Presse,
		ItemPick,
		ItemDrop,
		End
	};

public:
	MouseState GetMouseState() { return mState; }
	void SetMouseState(MouseState state) { mState = state; }

	void Update();
	
private:
	MouseState mState;
};

