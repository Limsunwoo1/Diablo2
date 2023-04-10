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

	void SetMesh(shared_ptr<Mesh> mesh) { mMesh = mesh; }
	void SetMaterial(shared_ptr<Material> material) { mMaterial = material; }
	shared_ptr<Mesh> GetMesh() { return mMesh; }
	shared_ptr<Material> GetMaterial() { return mMaterial; }

	void SetRenderStop(bool stop = true) { mRenderStop = stop; }
	bool GetRenderStop() { return mRenderStop; }
private:
	shared_ptr<Mesh> mMesh;
	shared_ptr<Material> mMaterial;
	bool mRenderStop;
};

