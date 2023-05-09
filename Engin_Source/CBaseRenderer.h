#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

using namespace std;
class BaseRenderer : public Component
{
public:
	BaseRenderer(eComponentType type);
	virtual ~BaseRenderer();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void SetMesh(weak_ptr<Mesh> mesh) { mMesh = mesh; }
	void SetMaterial(weak_ptr<Material> material) { mMaterial = material; }
	weak_ptr<Mesh> GetMesh() { return mMesh; }
	weak_ptr<Material> GetMaterial() { return mMaterial; }

	void SetRenderStop(bool stop = true) { mRenderStop = stop; }
	bool GetRenderStop() { return mRenderStop; }
private:
	weak_ptr<Mesh> mMesh;
	weak_ptr<Material> mMaterial;
	bool mRenderStop;
};

