#pragma once
#include "CSkilFire.h"

class SkilFireBall : public SkilFire
{
public:
	SkilFireBall(Player* player);
	virtual ~SkilFireBall();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

private:
};

