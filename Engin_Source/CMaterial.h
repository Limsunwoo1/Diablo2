#pragma once
#include "CResource.h"
#include "CShader.h"
#include "CRenderer.h"
#include "CTexture.h"

using namespace Renderer;
class Material : public Resource
{
public:
	Material();
	virtual ~Material();

	virtual HRESULT Load(const std::wstring& path) override;

	void SetData(eGpuParam param, void* data);
	void Bind();

	void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
	std::shared_ptr<Shader> GetShader() { return mShader; }
	void SetTexture(std::shared_ptr<Texture> texture) { mTexture = texture; }
	std::shared_ptr<Texture> GetTexture() { return mTexture; }

	void Clear();

private:
	std::shared_ptr<Shader> mShader;
	std::shared_ptr<Texture> mTexture;
	MaterialCB mCB;
};

