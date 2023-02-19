#pragma once
#include "CComponent.h"

using namespace Math;
using namespace std;
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
	void RegisterCameraInRenderer();

	void TrunLayerMast(eLayerType layer, bool enable = true);
	void EnableLayerMasks() { mLayerMasks.set(); }
	void DisableLayerMasks() { mLayerMasks.reset(); }

private:
	void SortGameObjects();
	void RenderOpaqu();
	void RenderCutOut();
	void RenderTransparent();
	void PushGameObjectToRenderingMode(GameObject* gameObject);

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

	bitset<(UINT)eLayerType::End> mLayerMasks;
	vector<GameObject*> mOpaquGameObjects;
	vector<GameObject*> mCutOutGameObjects;
	vector<GameObject*> mTransparentGameObjects;
};

