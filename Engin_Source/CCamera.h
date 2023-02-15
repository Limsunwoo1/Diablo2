#pragma once
#include "CComponent.h"

using namespace Math;
class Camera : public Component
{
public:
	enum class eProjectionType
	{
		Prespective,
		Orthographic,
	};
	__forceinline static Matrix& GetViewMatrix() { return View; }
	__forceinline static Matrix& GetProejctionMatrix() { return Projection; }

	Camera();
	~Camera();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void CreateViewMatrix();
	void CreateProjectionMatrix();

private:
	static Matrix View;
	static Matrix Projection;

	Matrix mView;
	Matrix mProjection;

	eProjectionType mType;
	float mAspectRatio; // Á¾È¾ºñ

	float mNear;
	float mFar;
	float mScale;

};

