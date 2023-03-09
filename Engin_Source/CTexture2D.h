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
		void BidShader(eShaderStage stage, UINT slot);
		void Clear();

	private:
		ScratchImage mImage;
		ComPtr<ID3D11Texture2D> mTexture;
		ComPtr<ID3D11ShaderResourceView> mSRV;
		D3D11_TEXTURE2D_DESC mDesc;
	};
}
