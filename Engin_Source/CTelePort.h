#pragma once
#include "CSkil.h"

class TelePort : public Skil
{
public:
	TelePort();
	virtual ~TelePort();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void InitAnimation() override;
	
	void SetMovePos(Vector3& pos) { mMovePos = pos; }
private:
	Vector3 mMovePos;
};

