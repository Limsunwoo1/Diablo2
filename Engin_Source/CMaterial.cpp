#include "CMaterial.h"

Material::Material()
    : Resource(eResourceType::Meterial)
	, mMode(eRenderingMode::Opaque)
{
}

Material::~Material()
{
}

HRESULT Material::Load(const std::wstring& path)
{
    return E_NOTIMPL;
}

void Material::SetData(eGpuParam param, void* data)
{
	switch (param)
	{
	case graphics::eGpuParam::Int:
		mCB.iData = *(static_cast<int*>(data));
		break;
	case graphics::eGpuParam::Float:
		mCB.fData = *(static_cast<float*>(data));
		break;
	case graphics::eGpuParam::Vector2:
		mCB.xy = *(static_cast<Vector2*>(data));
		break;
	case graphics::eGpuParam::Vector3:
		mCB.xyz = *(static_cast<Vector3*>(data));
		break;
	case graphics::eGpuParam::Vector4:
		mCB.xyzw = *(static_cast<Vector4*>(data));
		break;
	case graphics::eGpuParam::Matrix:
		mCB.matrix = *(static_cast<Matrix*>(data));
		break;
	default:
		break;
	}
}

void Material::Bind()
{
	for (int i = 0; i < (UINT)eTextureSlot::End; ++i)
	{
		if (mTexture[i] == nullptr)
			continue;

		mTexture[i]->BindShaderResource(eShaderStage::VS, i);
		mTexture[i]->BindShaderResource(eShaderStage::HS, i);
		mTexture[i]->BindShaderResource(eShaderStage::DS, i);
		mTexture[i]->BindShaderResource(eShaderStage::GS, i);
		mTexture[i]->BindShaderResource(eShaderStage::PS, i);
		mTexture[i]->BindShaderResource(eShaderStage::CS, i);
	}

	ConstantBuffer* pCB = Renderer::constantBuffers[(UINT)eCBType::Material];

	pCB->SetData(&mCB);
	pCB->Bind(eShaderStage::VS);
	pCB->Bind(eShaderStage::GS);
	pCB->Bind(eShaderStage::PS);

	mShader->Binds();
}

void Material::Clear()
{
	for (int i = 0; i < (UINT)eTextureSlot::End; ++i)
	{
		if (mTexture[i] == nullptr)
			continue;

		mTexture[i]->Clear(i);
	}
}
