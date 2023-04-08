#pragma once
#include "CGameObject.h"
#include "Cplayer.h"

class Skil : public GameObject
{
public:
	Skil();
	Skil(Player* owner);
	virtual ~Skil();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override {};

	void SetOwner(Player* owner) { mOwner = owner; }

protected:
	Player* mOwner;
};

