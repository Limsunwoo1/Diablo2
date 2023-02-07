#pragma once
#include "CResource.h"
#include "CShader.h"
#include "CRenderer.h"

using namespace Renderer;
class Material : public Resource
{
public:
	Material();
	~Material();

	virtual HRESULT Load(const std::wstring& path) override;

	void SetData(eGpuParam param, void* data);
	void Bind();

	void SetShader(Shader* shader) { mShader = shader; }
	Shader* GetShader() { return mShader; }

private:
	Shader* mShader;
	MaterialCB mCB;
};

