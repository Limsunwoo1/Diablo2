#pragma once
#include "CBoltBase.h"

class FrozenBolt : public BoltBase
{
public:
	FrozenBolt();
	~FrozenBolt();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;

private:

};