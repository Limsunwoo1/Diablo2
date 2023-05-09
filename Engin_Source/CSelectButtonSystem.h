#pragma once
#include "CUiBase.h"

using namespace std;
using namespace Math;

class SelectButtonSystem : public UiBase
{
public:
	SelectButtonSystem();
	virtual ~SelectButtonSystem();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;


private:
	vector<class CharacterSelectScene> mButton;
	int mClickButtonIdx;
};

