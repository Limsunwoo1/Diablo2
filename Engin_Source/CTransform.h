#pragma once
#include "CComponent.h"

using namespace Math;

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void SetConstantBuffer();

	void SetParent(Transform* parent) { mParent = parent; }
	Transform* GetParent() { return mParent; }

	Vector3 GetPosition() { return mPosition; }
	Vector3 GetRotation() { return mRotation; }
	Vector3 GetScale() { return mScale; }
	Vector3 GetSize() { return mSize; }

	void SetPosition(Vector3 position) { mPosition = position; }
	void SetPosition(float x, float y, float z) { mPosition = Vector3(x, y, z); }

	void SetRotation(Vector3 degree) { mRotation = degree; }
	void SetScale(Vector3 scale) { mScale = scale; }
	void SetSize(Vector3 size) { mSize = size; }

	Vector3 Forward() { return mForward; }
	Vector3 Right() { return mRight; }
	Vector3 Up() { return mUp; }

	Matrix& GetWorldMatrix() { return mWorld; }
	void SetWorldMatrix(Matrix& mat) { mWorld = mat; }
private:
	Transform* mParent;

	Vector3 mForward;
	Vector3 mRight;
	Vector3 mUp;

	Vector3 mPosition;
	Vector3 mRotation;
	Vector3 mScale;
	Vector3 mSize;

	Matrix mWorld;
};