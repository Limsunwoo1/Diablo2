#include "CMouseManager.h"

MouseManager::MouseManager()
{

}

MouseManager::~MouseManager()
{

}

void MouseManager::Update()
{
	//if (GetFocus())
	//{
	//	// Ű����
	//	for (UINT i = 0; i < (UINT)eKeyCode::End; i++)
	//	{
	//		// �ش�Ű�� ���� ������ �ִ°��
	//		if (GetAsyncKeyState(ASCII[i]) & 0x8000)
	//		{
	//			// ���� �����ӿ� ������ �־���
	//			if (mKeys[i].bPressed)
	//				mKeys[i].state = eKeyState::PRESSED;
	//			else
	//				mKeys[i].state = eKeyState::DOWN;

	//			mKeys[i].bPressed = true;
	//		}
	//		// �ش�Ű�� ������ ���� ���� ���
	//		else
	//		{
	//			// ���� �����ӿ� ������ �־���
	//			if (mKeys[i].bPressed)
	//				mKeys[i].state = eKeyState::UP;
	//			else
	//				mKeys[i].state = eKeyState::NONE;

	//			mKeys[i].bPressed = false;
	//		}
	//	}
	//}
}
