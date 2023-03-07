#pragma once
#include "CComponent.h"

using namespace Math;

class Transform;
class Collider2D : public Component
{
public:
	Collider2D();
	~Collider2D();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	virtual void OnCollisionEnter(Collider2D* collider);
	virtual void OnCollisionStay(Collider2D* collider);
	virtual void OnCollisionExit(Collider2D* collider);

	virtual void OnTriggerEnter(Collider2D* collider);
	virtual void OnTriggerStay(Collider2D* collider);
	virtual void OnTriggerExit(Collider2D* collider);


	void SetType(eColliderType type) { mType = type; }
	void SetSize(Vector2 size) { mSize = size; }
	void SetCenter(Vector2 center) { mCenter = center; }
	bool IsTrigger() { return mbTrigger; }
	
private:
	eColliderType mType;
	Transform* mTransform;

	Vector2 mSize;
	Vector2 mCenter;

	bool mbTrigger;
};

