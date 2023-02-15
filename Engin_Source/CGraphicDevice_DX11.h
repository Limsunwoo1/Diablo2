#pragma once
#include "Graphics.h"

using namespace Microsoft::WRL;

namespace graphics
{
	class CGraphicDevice_DX11
	{
	public:
		CGraphicDevice_DX11(ValidationMode validationMode = ValidationMode::Disabled);
		~CGraphicDevice_DX11();

		bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC* desc);
		bool CreateTextur(D3D11_TEXTURE2D_DESC* desc, ID3D11Texture2D** ppTexture2D);

		bool CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT NumElements, const void* byteCode, SIZE_T bytecodeLength, ID3D11InputLayout** ppInputLayout);
		bool CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data, ID3D11Buffer** buffer);
		bool CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader);
		bool CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppVertexShader);
		bool CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);

		void BindPrivitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
		void BindInputLayout(ID3D11InputLayout* pInputLayout);
		void BindVertexBuffer(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
		void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
		void BindVertexShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindPixelShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindViewPorts(D3D11_VIEWPORT* viewPort);
		void BindConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size);
		void SetConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);
		void SetShaderResource(eShaderStage stage, UINT slot, ID3D11ShaderResourceView* const* ppShaderResourceViews);
		void BindSamplers(eShaderStage stage, UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);
		void BindsSamplers(UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);

		void Clear();
		void AdjustViewPorts();

		void Draw();
		void DrawIndexed(UINT indexCount, UINT StartIndexLocation, UINT BaseVertexLocation);

		void Present();

		ID3D11Device* GetID3D11Device() { return mDevice.Get(); }

	private:
		// GPU 객체 생성 그래픽카드와 연결되는 기본적인객체
		ComPtr <ID3D11Device> mDevice;
		// GPU read write 디바이스에 직접 접근하지 않고 이 객체를 통해 GPU에 명령을 내린다
		ComPtr <ID3D11DeviceContext> mContext;

		// 최종적으로 그려지는 도화지
		ComPtr <ID3D11Texture2D> mRenderTarget;
		// 렌더타겟 객체에 접근하기위한 객체 (직접접근X View를 통한 우회접근)
		ComPtr <ID3D11RenderTargetView> mRenderTargetView; // 렌더 타겟 접근 권한 - iter 같은 개념

		ComPtr <ID3D11Texture2D> mDepthStencilBuffer;
		// 마찬가지로 딮 스텐실 버퍼에 접근하기위한 객체
		ComPtr <ID3D11DepthStencilView> mDepthStencilView;

		// 화면에 최정적으로 그려지는
		// 백퍼버(Frame Buffer)를 관리하고, 실제로 화면에 렌더링 하는
		// 역할을 담당하는 객체
		ComPtr <IDXGISwapChain> mSwapChain;

		// 텍스쳐 로딩할때 사용된다.
		// 안티엘리어싱 (계단현상 방지)
		//ID3D11SamplerState* mSampler;

		D3D11_VIEWPORT mViewPort;
	};

	inline CGraphicDevice_DX11*& GetDevice()
	{
		static CGraphicDevice_DX11* device = nullptr;
		return device;
	}
}

