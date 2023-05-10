#pragma once
#include "CButton.h"

using namespace std;

class SelectSystemButton : public Button
{
public:
	SelectSystemButton();
	virtual ~SelectSystemButton();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void SetButtonTex(UINT slot, weak_ptr<Texture2D> tex);
private:
	vector<weak_ptr<Texture2D>> mButtonTex;
};

