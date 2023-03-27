#pragma once
#include "CGameObject.h"
#include "Cplayer.h"

class SkilFire : public GameObject
{
public:
	SkilFire(Player* owner);
	virtual ~SkilFire();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override {};

protected:
	Player* mOwner;
};

