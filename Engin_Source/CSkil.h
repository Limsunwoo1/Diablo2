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

	void SetElementType(eElementType type) { mElementType = type; }
	eElementType GetElementType() { return mElementType; }

	float GetDamege() { return mDamege; }
	void SetDamege(float damege) { mDamege = damege; }

	virtual void HitSkil(GameObject* obj) {};

protected:
	Player* mOwner;
	eElementType mElementType;
	float mCost;

	float mDamege;
};

