#pragma once
#include "CButton.h"


using namespace std;
using namespace Math;

class PlayerSelectButton : public Button
{
public:
	PlayerSelectButton();
	virtual ~PlayerSelectButton();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

private:
	GameObject* mCharterAnimation;
};

