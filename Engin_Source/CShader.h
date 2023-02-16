#pragma once
#include "CResource.h"
#include "Graphics.h"

using namespace Microsoft::WRL;
using namespace graphics;
class Shader : public Resource
{
public:
	Shader();
	~Shader();

	virtual HRESULT Load(const std::wstring& path) override;

	void Create(graphics::eShaderStage stage, const std::wstring& file, const std::string& funcName);
	void Binds();

	ID3D11InputLayout* GetInputLayout() { return mInputLayout.Get(); }
	ID3D11InputLayout** GetInputLayoutAddressOf() { return mInputLayout.GetAddressOf(); }

	void* GetVSBlobBufferPointer() { return mVSBlob->GetBufferPointer(); }
	SIZE_T GetVSBlobBufferSize() { return mVSBlob->GetBufferSize(); }

	void SetRasterize(eRasterizeType type) { mRSType = type; }
	void SetDepthStencil(eDepthStencilType type) { mDSType = type; }
	void SetBlend(eBlendType type) { mBLType = type; }

private:
	ComPtr<ID3D11InputLayout> mInputLayout;
	D3D11_PRIMITIVE_TOPOLOGY mTopology;

	ComPtr <ID3DBlob> mVSBlob;
	ComPtr <ID3DBlob> mHSBlob;
	ComPtr <ID3DBlob> mDSBlob;
	ComPtr <ID3DBlob> mGSBlob;
	ComPtr <ID3DBlob> mPSBlob;


	ComPtr <ID3D11VertexShader>		mVS;
	ComPtr <ID3D11HullShader>		mHS;
	ComPtr <ID3D11DomainShader>		mDS;
	ComPtr <ID3D11GeometryShader>	mGS;
	ComPtr <ID3D11PixelShader>		mPS;

	eRasterizeType		mRSType;
	eDepthStencilType	mDSType;
	eBlendType			mBLType;

	ComPtr <ID3DBlob> mErrorBlob;
};

