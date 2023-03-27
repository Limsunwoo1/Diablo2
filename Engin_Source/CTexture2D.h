#pragma once
#include "CResource.h"
#include "..\External\DirectXTex\Include\DirectXTex.h"
#include "CGraphicDevice_DX11.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\External\\DirectXTex\\lib\\Debug\\DirectXTex.lib")
#else
#pragma comment(lib, "..\\External\\DirectXTex\\lib\\Release\\DirectXTex.lib")
#endif

using namespace Microsoft::WRL;

namespace graphics
{
	class Texture2D : public Resource
	{
	public:
		Texture2D();
		virtual ~Texture2D();

		virtual HRESULT Load(const std::wstring& path) override;
		void BindShader(eShaderStage stage, UINT slot);
		void BIndUnorderedAccessView(UINT startSlot);
		void ClearUnorderedAccessView(UINT startSlot);

		bool Create(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlag);
		bool Create(ComPtr<ID3D11Texture2D> texture);

		void Clear();
		void Clear(UINT startSlot);

		UINT GetHeight() { return mDesc.Height; }
		UINT GetWidth() { return mDesc.Width; }

		ComPtr<ID3D11Texture2D> GetTexture()		{ return mTexture; }
		ComPtr<ID3D11DepthStencilView> GetDSV()		{ return mDSV; }
		ComPtr<ID3D11RenderTargetView> GetRTV()		{ return mRTV; }
		ComPtr<ID3D11UnorderedAccessView> GetUAV()	{ return mUAV; }
		ComPtr<ID3D11ShaderResourceView> GetSRV()	{ return mSRV; }

	private:
		ScratchImage mImage;
		ComPtr<ID3D11Texture2D> mTexture;
		ComPtr<ID3D11DepthStencilView> mDSV;
		ComPtr<ID3D11RenderTargetView> mRTV;
		ComPtr<ID3D11UnorderedAccessView> mUAV;
		ComPtr<ID3D11ShaderResourceView> mSRV;
		D3D11_TEXTURE2D_DESC mDesc;
	};
}
