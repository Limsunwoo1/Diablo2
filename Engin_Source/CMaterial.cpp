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
	if(mTexture)
		mTexture->BidShader(eShaderStage::PS, 0);

	ConstantBuffer* pCB = Renderer::constantBuffers[(UINT)eCBType::Material];

	pCB->Bind(&mCB);
	pCB->SetPipline(eShaderStage::VS);
	pCB->SetPipline(eShaderStage::PS);

	mShader->Binds();
}

void Material::Clear()
{
	if(!mTexture)
		mTexture->Clear();
}
