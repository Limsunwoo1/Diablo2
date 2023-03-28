#pragma once
#include "Graphics.h"

using namespace Microsoft::WRL;
using namespace std;

namespace graphics
{
	class CGraphicDevice_DX11
	{
	public:
		CGraphicDevice_DX11(ValidationMode validationMode = ValidationMode::Disabled);
		~CGraphicDevice_DX11();

		bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC* desc);
		bool CreateTexture(D3D11_TEXTURE2D_DESC* desc, ID3D11Texture2D** ppTexture2D);

		bool CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT NumElements, const void* byteCode, SIZE_T bytecodeLength, ID3D11InputLayout** ppInputLayout);
		bool CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data, ID3D11Buffer** buffer);
		bool CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView);
		bool CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView);
		bool CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView);
		bool CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView);
		bool CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader);
		bool CreateHullShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11HullShader** ppHullShader);
		bool CreateDomainShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11DomainShader** ppDomainShader);
		bool CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader);
		bool CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppVertexShader);
		bool CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader);
		bool CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);
		bool CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerdesc, ID3D11RasterizerState** ppRasterizerState);
		bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencildesc, ID3D11DepthStencilState** ppDepthStencilState);
		bool CreateBlendState(const D3D11_BLEND_DESC* pBlenddesc, ID3D11BlendState** ppBlendState);

		void BindPrivitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
		void BindInputLayout(ID3D11InputLayout* pInputLayout);
		void BindVertexBuffer(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
		void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
		void BindVertexShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindHullShader(ID3D11HullShader* pHullShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindDomainShader(ID3D11DomainShader* pDomainShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindGeometryShader(ID3D11GeometryShader* pGeometryShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindPixelShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindComputeShader(ID3D11ComputeShader* pComputeShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindBuffer(ID3D11Buffer* buffer, void* data, UINT size);
		void BindViewPorts(D3D11_VIEWPORT* viewPort);
		void Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ);
		void BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);
		void BindShaderResource(eShaderStage stage, UINT slot, ID3D11ShaderResourceView* const* ppShaderResourceViews);
		void BindUnorderdAccessView(UINT slot, UINT numUAVs, ID3D11UnorderedAccessView* const* ppUAV, const UINT* pUAVInitalCounts);
		void BindSamplers(eShaderStage stage, UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);
		void BindsSamplers(UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);
		void BindRasterizerState(ID3D11RasterizerState* pRasterizerState);
		void BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState);
		void BindBlendState(ID3D11BlendState* pBlendState);

		void Clear();
		void AdjustViewPorts();

		void Draw();
		void DrawIndexed(UINT indexCount, UINT StartIndexLocation, UINT BaseVertexLocation);
		void DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation);

		void Present();

		ID3D11Device* GetID3D11Device() { return mDevice.Get(); }
		ID3D11DeviceContext* GetID3D11DeviceContext() { return mContext.Get(); }

	private:
		// GPU 객체 생성 그래픽카드와 연결되는 기본적인객체
		ComPtr<ID3D11Device> mDevice;
		// GPU read write 디바이스에 직접 접근하지 않고 이 객체를 통해 GPU에 명령을 내린다
		ComPtr<ID3D11DeviceContext> mContext;

		shared_ptr<class Texture2D> mRenderTargetTexture;
		shared_ptr<class Texture2D> mDepthStencilBufferTexture;

		// 화면에 최정적으로 그려지는
		// 백퍼버(Frame Buffer)를 관리하고, 실제로 화면에 렌더링 하는
		// 역할을 담당하는 객체
		ComPtr<IDXGISwapChain> mSwapChain;

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

