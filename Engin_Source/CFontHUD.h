#pragma once
#include "CHUD.h"

class FontHUD : public HUD
{
public:
	FontHUD();
	virtual ~FontHUD();


	virtual void Render() override;
	virtual void Initalize() override;

	void SetCurNum(int num) { mCurNum = num; }
	void SetMaxNum(int num) { mMaxNum = num; }

	int GetCurNum() { return mCurNum; }
	int GetMaxNum() { return mMaxNum; }


private:

	// ���� �ε���
	int mCurNum;

	// ��ü ����
	int mMaxNum;
};
