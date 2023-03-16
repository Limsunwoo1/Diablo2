#pragma once
#include "CComponent.h"
#include "Graphics.h"


using namespace Math;

class Light : public Component
{
public:
	Light();
	virtual ~Light();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	graphics::LightAttribute GetLightAttribute() { return mAttribute; }

	Vector4 GetDiffuse() { return mAttribute.diffuse; }
	void SetDiffuse(Vector4 diffuse) { mAttribute.diffuse = diffuse; }

	float GetRadius() { return mAttribute.radius; }
	void SetRadius(float radius) { mAttribute.radius = radius; }

	float GetAngle() { return mAttribute.angle; }
	void SetAngle(float angle) { mAttribute.angle = angle; }

	eLightType GetType() { return mAttribute.type; }
	void SetType(eLightType type) { mAttribute.type = type; }

private:
	graphics::LightAttribute mAttribute;
};