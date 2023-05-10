#pragma once
#include "CButton.h"


using namespace std;
using namespace Math;

class PlayerSelectButton : public Button
{
public:
	struct PlayerInfo
	{
		UINT			CharterType;
		UINT			Level;
		float			Hp;
		float			Mp;

		float			Exp;
		Vector3			Postion;

		wstring	Name;
	};

public:
	PlayerSelectButton();
	virtual ~PlayerSelectButton();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void Click() override;

	eCharType GetCharType() { return (eCharType)mChartType; }
	void SetChrType(eCharType type) { mChartType = (UINT)type; }

	const PlayerInfo& GetInfo() { return mInfo; }
	void SetInfo(wstring& name ,UINT type, UINT level, float hp, float mp, float exp, Vector3 pos);
	void SetInfo(PlayerInfo info) { mInfo = info; }

	void SetSystem(class SelectButtonSystem* system) { mSystem = system; }
	SelectButtonSystem* GetSystem() { return mSystem; }

	int GetButtonIndex() { return mButtonIndex; }
	void SetButtonIndex(UINT index) { mButtonIndex = index; }

	int GetDataPathIndex() { return mDataPathIndex; }
	void SetDataPathIndex(UINT index) { mDataPathIndex = index; }

private:
	GameObject* mCharterAnimation;
	SelectButtonSystem* mSystem;

	UINT mChartType;
	int mButtonIndex;

	PlayerInfo mInfo;

	int mDataPathIndex;
};

