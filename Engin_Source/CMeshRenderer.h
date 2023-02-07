#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CMaterial.h"

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void Initalize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void SetMesh(Mesh* mesh) {mMesh = mesh;}
	void SetMaterial(Material* material) { mMaterial = material;}

private:
	Mesh* mMesh;
	Material* mMaterial;
};