#pragma once
#include "CBoltBase.h"

class FireBolt : public BoltBase
{
public:
	FireBolt();
	~FireBolt();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

private:

};
