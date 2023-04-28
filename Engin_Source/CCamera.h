#pragma once
#include "CComponent.h"

using namespace Math;
using namespace std;
class GameObject;
class Camera : public Component
{
public:
	enum class eProjectionType
	{
		Prespective,
		Orthographic,
	};
	__forceinline static Matrix& GetGpuViewMatrix() { return View; }
	__forceinline static Matrix& GetGpuProejctionMatrix() { return Projection; }
	__forceinline static void SetGpuViewMatrix(Matrix view) { View = view; }
	__forceinline static void SetGpuProjectionMatrix(Matrix projection) { Projection = projection; }

	Camera();
	~Camera();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void CreateViewMatrix();
	void CreateProjectionMatrix();
	void RegisterCameraInRenderer();

	void TurnLayerMask(eLayerType layer, bool enable = true);
	void EnableLayerMasks() { mLayerMasks.set(); }
	void DisableLayerMasks() { mLayerMasks.reset(); }

	void SetProjectionType(eProjectionType type) { mType = type; }

	float GetScale() { return mScale; }
	Matrix& GetViewMatrix() { return mView; }
	Matrix& GetProjectionMatrix() { return mProjection; }

	void SetTrace(GameObject* obj) { mTrace = obj; }
	GameObject* GetTrace() { return mTrace; }

private:
	void SortGameObjects();
	void RenderOpaqu();
	void RenderCutOut();
	void RenderTransparent();
	void RenderPostProcess();
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
	vector<GameObject*> mPostProcessGameObjects;

	GameObject* mTrace;
};

