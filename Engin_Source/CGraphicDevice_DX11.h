#pragma once
#include "Graphics.h"

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
		bool CreateShader();

		void BindVertexBuffer(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
		void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
		void BindViewPorts(D3D11_VIEWPORT* viewPort);
		void BindConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size);
		void SetConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);

		void Clear();
		void AdjustViewPorts();

		void Draw();
		void DrawIndexed(UINT indexCount, UINT StartIndexLocation, UINT BaseVertexLocation);

		void Present();
		void Render();

	private:
		// GPU 객체 생성 그래픽카드와 연결되는 기본적인객체
		Microsoft::WRL::ComPtr <ID3D11Device> mDevice;
		// GPU read write 디바이스에 직접 접근하지 않고 이 객체를 통해 GPU에 명령을 내린다
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> mContext;

		// 최종적으로 그려지는 도화지
		Microsoft::WRL::ComPtr <ID3D11Texture2D> mRenderTarget;
		// 렌더타겟 객체에 접근하기위한 객체 (직접접근X View를 통한 우회접근)
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> mRenderTargetView; // 렌더 타겟 접근 권한 - iter 같은 개념

		Microsoft::WRL::ComPtr <ID3D11Texture2D> mDepthStencilBuffer;
		// 마찬가지로 딮 스텐실 버퍼에 접근하기위한 객체
		Microsoft::WRL::ComPtr <ID3D11DepthStencilView> mDepthStencilView;

		// 화면에 최정적으로 그려지는
		// 백퍼버(Frame Buffer)를 관리하고, 실제로 화면에 렌더링 하는
		// 역할을 담당하는 객체
		Microsoft::WRL::ComPtr <IDXGISwapChain> mSwapChain;

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

