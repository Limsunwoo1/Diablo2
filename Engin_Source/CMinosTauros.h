#pragma once
#include "CMonster.h"

class MinosTauros : public Monster
{
public:
	MinosTauros();
	virtual ~MinosTauros();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

private:

};

