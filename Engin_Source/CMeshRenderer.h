#pragma once
#include "CComponent.h"
#include "CMesh.h"
#include "CShader.h"

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
	void SetShader(Shader* shader) { mShader = shader;}
private:
	Mesh* mMesh;
	Shader* mShader;
};