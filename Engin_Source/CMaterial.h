#pragma once
#include "CResource.h"
#include "CShader.h"
#include "CRenderer.h"
#include "CTexture2D.h"

using namespace Renderer;
using namespace graphics;

class Material : public Resource
{
public:
	Material();
	virtual ~Material();

	virtual HRESULT Load(const std::wstring& path) override;

	void SetData(eGpuParam param, void* data);
	void Bind();
	void Clear();

	void SetShader(std::weak_ptr<Shader> shader) { mShader = shader; }
	std::weak_ptr<Shader> GetShader() { return mShader; }
	void SetTexture(eTextureSlot slot, std::weak_ptr<Texture2D> texture) { mTexture[(UINT)slot] = texture; }
	std::weak_ptr<Texture2D> GetTexture(eTextureSlot slot) { return mTexture[(UINT)slot]; }

	eRenderingMode GetRenderingMode() { return mMode; }
	void SetRenderingMode(eRenderingMode mode) { mMode = mode; }
private:
	std::weak_ptr<Shader> mShader;
	std::weak_ptr<Texture2D> mTexture[(UINT)eTextureSlot::End];
	MaterialCB mCB;
	eRenderingMode mMode;
};

