#include "CGraphicDevice_DX11.h"
#include "CApplication.h"
#include "CRenderer.h"
#include "CTexture2D.h"
#include "CResourceManager.h"

extern CApplication Application;

namespace graphics
{
	CGraphicDevice_DX11::CGraphicDevice_DX11(ValidationMode validationMode)
	{
		graphics::GetDevice() = this;
		// 1. Device �� SwapChain �� �����Ѵ�
		// 2. ����ۿ� ������ ������ �� ����Ÿ�� �並 �����ؾ� �Ѵ�.
		// 3. ȭ���� Ŭ���� ������Ѵ�. ����Ʈ�� ��������� �Ѵ�.
		// 4. �������Ӹ��� ������ ������ ����Ÿ�ٺ信 ���������־���Ѵ�.
		// 5. SwapChain �� �̿��Ͽ� ���� ����̽�(���÷���)�� ȭ���� �׷���� �Ѵ�.

		HWND hwnd = Application.GetHwnd();

		// Device, Device Context
		UINT DeviceFlag = D3D11_CREATE_DEVICE_DEBUG; // �⺻ ��� Ű��
		D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0; // �⺻ ���

		/*HRESULT WINAPI D3D11CreateDevice(
			_In_opt_ IDXGIAdapter * pAdapter,
			D3D_DRIVER_TYPE DriverType,
			HMODULE Software,
			UINT Flags,
			_In_reads_opt_(FeatureLevels) CONST D3D_FEATURE_LEVEL * pFeatureLevels,
			UINT FeatureLevels,
			UINT SDKVersion,
			_COM_Outptr_opt_ ID3D11Device * *ppDevice,
			_Out_opt_ D3D_FEATURE_LEVEL * pFeatureLevel,
			_COM_Outptr_opt_ ID3D11DeviceContext * *ppImmediateContext);*/

		// ����̽� ������
		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
										, DeviceFlag, nullptr, 0
										, D3D11_SDK_VERSION
										, mDevice.GetAddressOf()
										, &FeatureLevel
										, mContext.GetAddressOf());

		// ����ü�� ����
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		swapChainDesc.OutputWindow = hwnd;												 // �ڵ� ��
		swapChainDesc.Windowed = true;													 // â��� ����
		swapChainDesc.BufferCount = 2;													 // ������ ���� (�⺻���� ������ ����)
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
		//DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
		swapChainDesc.BufferDesc.Width = Application.GetWidth();
		swapChainDesc.BufferDesc.Height = Application.GetHeight();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;		  // 32 ��Ʈ ���
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 144;							  // �ֻ��� �ִ밪
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;							  // �ֻ��� �ּڰ�
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				  // ���� �� ����
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // �����Ͷ����� ���� ����

		swapChainDesc.SampleDesc.Count = 1;												  // ���ø�
		swapChainDesc.SampleDesc.Quality = 0;											  // ����Ƽ �⺻

		if (!CreateSwapChain(&swapChainDesc))											  // ����ü�� ����
			return;

		mRenderTargetTexture = make_shared<Texture2D>();
		ResourceManager::GetInstance()->Insert<Texture2D>(L"RenderTarget", mRenderTargetTexture);
		ComPtr<ID3D11Texture2D> renderTarget;

		// Get Render Target for SwapChain
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)renderTarget.GetAddressOf());

		mRenderTargetTexture->Create(renderTarget);



		D3D11_TEXTURE2D_DESC depthBuffer = {};
		depthBuffer.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthBuffer.Usage = D3D11_USAGE_DEFAULT;										  // Default ��
		depthBuffer.CPUAccessFlags = 0;													  // CPU �� �ʿ��� ��Ȳ���� ������

		depthBuffer.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;				  // ����ä���� �����ϴ� 8��Ʈ 4�� �� 32��Ʈ
		depthBuffer.Width = Application.GetWidth();										  
		depthBuffer.Height = Application.GetHeight();
		depthBuffer.ArraySize = 1;			
		// ���� ����
		depthBuffer.SampleDesc.Count = 1;
		depthBuffer.SampleDesc.Quality = 0;

		depthBuffer.MipLevels = 0;														  // ������� ������ ���̱����� �̸� ������ ������ �� �׷���
		depthBuffer.MiscFlags = 0;

		mDepthStencilBufferTexture = make_shared<Texture2D>();
		mDepthStencilBufferTexture->Create(1600, 900, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

		RECT winRect;
		GetClientRect(Application.GetHwnd(), &winRect);
		mViewPort = { 0.0f, 0.0f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.0f, 1.0f };
		BindViewPorts(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetTexture->GetRTV().GetAddressOf(), mDepthStencilBufferTexture->GetDSV().Get());
	}

	CGraphicDevice_DX11::~CGraphicDevice_DX11()
	{
		Renderer::Release();
	}
	bool CGraphicDevice_DX11::CreateSwapChain(DXGI_SWAP_CHAIN_DESC* desc)
	{
		Microsoft::WRL::ComPtr<IDXGIDevice>  pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return false;

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdapter.GetAddressOf())))
			return false;

		if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf())))
			return false;

		// ����ü�� ����
		if (FAILED(pDXGIFactory->CreateSwapChain(mDevice.Get(), desc, mSwapChain.GetAddressOf())))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateTexture(D3D11_TEXTURE2D_DESC* desc, ID3D11Texture2D** ppTexture2D)
	{
		if (FAILED(mDevice->CreateTexture2D(desc, nullptr, ppTexture2D)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT NumElements, const void* byteCode, SIZE_T bytecodeLength, ID3D11InputLayout** ppInputLayout)
	{
		if (FAILED(mDevice->CreateInputLayout(desc, NumElements, byteCode, bytecodeLength, ppInputLayout)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data, ID3D11Buffer** buffer)
	{
		// System -> GPU
		// GPU�� ���� �����͸� ���۶�� ��
		if (FAILED(mDevice->CreateBuffer(desc, data, buffer)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView)
	{
		if (FAILED(mDevice->CreateRenderTargetView(pResource, pDesc, ppRTView)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView)
	{
		if (FAILED(mDevice->CreateUnorderedAccessView(pResource, pDesc, ppUAView)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView)
	{
		if (FAILED(mDevice->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
	{
		if (FAILED(mDevice->CreateShaderResourceView(pResource, pDesc, ppSRView)))
			return false;

		return true;
	}

	bool CGraphicDevice_DX11::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
	{
		if (FAILED(mDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader)))
			return false;

		return true;
	}

	bool CGraphicDevice_DX11::CreateHullShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11HullShader** ppHullShader)
	{
		if (FAILED(mDevice->CreateHullShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader)))
			return false;

		return true;
	}

	bool CGraphicDevice_DX11::CreateDomainShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11DomainShader** ppDomainShader)
	{
		if (FAILED(mDevice->CreateDomainShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader)))
			return false;

		return true;
	}

	bool CGraphicDevice_DX11::CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader)
	{
		if (FAILED(mDevice->CreateGeometryShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader)))
			return false;

		return true;
	}

	bool CGraphicDevice_DX11::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppVertexShader)
	{
		if (FAILED(mDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader)
	{
		if (FAILED(mDevice->CreateComputeShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
	{
		if (FAILED(mDevice->CreateSamplerState(pSamplerDesc, ppSamplerState)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerdesc, ID3D11RasterizerState** ppRasterizerState)
	{
		if (FAILED(mDevice->CreateRasterizerState(pRasterizerdesc, ppRasterizerState)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencildesc, ID3D11DepthStencilState** ppDepthStencilState)
	{
		if (FAILED(mDevice->CreateDepthStencilState(pDepthStencildesc, ppDepthStencilState)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateBlendState(const D3D11_BLEND_DESC* pBlenddesc, ID3D11BlendState** ppBlendState)
	{
		if (FAILED(mDevice->CreateBlendState(pBlenddesc, ppBlendState)))
			return false;

		return true;
	}
	void CGraphicDevice_DX11::BindPrivitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		mContext->IASetPrimitiveTopology(topology);
	}
	void CGraphicDevice_DX11::BindInputLayout(ID3D11InputLayout* pInputLayout)
	{
		mContext->IASetInputLayout(pInputLayout);
	}
	void CGraphicDevice_DX11::BindVertexBuffer(UINT StartSlot
		, UINT NumBuffers
		, ID3D11Buffer* const* ppVertexBuffers
		, const UINT* pStrides
		, const UINT* pOffsets)
	{
		mContext->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
	}
	void CGraphicDevice_DX11::BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
	{
		mContext->IASetIndexBuffer(pIndexBuffer, Format, Offset);
	}
	void CGraphicDevice_DX11::BindVertexShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
	}
	void CGraphicDevice_DX11::BindHullShader(ID3D11HullShader* pHullShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->HSSetShader(pHullShader, ppClassInstances, NumClassInstances);
	}
	void CGraphicDevice_DX11::BindDomainShader(ID3D11DomainShader* pDomainShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->DSSetShader(pDomainShader, ppClassInstances, NumClassInstances);
	}
	void CGraphicDevice_DX11::BindGeometryShader(ID3D11GeometryShader* pGeometryShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->GSSetShader(pGeometryShader, ppClassInstances, NumClassInstances);
	}
	void CGraphicDevice_DX11::BindPixelShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
	}
	void CGraphicDevice_DX11::BindComputeShader(ID3D11ComputeShader* pComputeShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->CSSetShader(pComputeShader, ppClassInstances, NumClassInstances);
	}
	void CGraphicDevice_DX11::BindViewPorts(D3D11_VIEWPORT* viewPort)
	{
		mContext->RSSetViewports(1, viewPort);
	}
	void CGraphicDevice_DX11::Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ)
	{
		mContext->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
	}
	void CGraphicDevice_DX11::BindBuffer(ID3D11Buffer* buffer, void* data, UINT size)
	{
		D3D11_MAPPED_SUBRESOURCE sub = {};
		mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, data, size);
		mContext->Unmap(buffer, 0);
	}
	void CGraphicDevice_DX11::BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
	{
		switch (stage)
		{
		case graphics::eShaderStage::VS:
			mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case graphics::eShaderStage::HS:
			mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case graphics::eShaderStage::DS:
			mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case graphics::eShaderStage::GS:
			mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case graphics::eShaderStage::PS:
			mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case graphics::eShaderStage::CS:
			mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		default:
			break;
		}
	}
	void CGraphicDevice_DX11::BindShaderResource(eShaderStage stage, UINT slot, ID3D11ShaderResourceView* const* ppShaderResourceViews)
	{
		switch (stage)
		{
		case graphics::eShaderStage::VS:
			mContext->VSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case graphics::eShaderStage::HS:
			mContext->HSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case graphics::eShaderStage::DS:
			mContext->DSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case graphics::eShaderStage::GS:
			mContext->GSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case graphics::eShaderStage::PS:
			mContext->PSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case graphics::eShaderStage::CS:
			mContext->CSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		default:
			break;
		}
	}
	void CGraphicDevice_DX11::BindUnorderdAccessView(UINT slot, UINT numUAVs, ID3D11UnorderedAccessView* const* ppUAV, const UINT* pUAVInitalCounts)
	{
		mContext->CSSetUnorderedAccessViews(slot, numUAVs, ppUAV, pUAVInitalCounts);
	}
	void CGraphicDevice_DX11::BindSamplers(eShaderStage stage, UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers)
	{
		switch (stage)
		{
		case graphics::eShaderStage::VS:
			mContext->VSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case graphics::eShaderStage::HS:
			mContext->HSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case graphics::eShaderStage::DS:
			mContext->DSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case graphics::eShaderStage::GS:
			mContext->GSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case graphics::eShaderStage::PS:
			mContext->PSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case graphics::eShaderStage::CS:
			mContext->CSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		default:
			break;
		}
	}
	void CGraphicDevice_DX11::BindsSamplers(UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers)
	{
		mContext->VSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->HSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->DSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->GSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->PSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->CSSetSamplers(slot, NumSamplers, ppSamplers);
	}
	void CGraphicDevice_DX11::BindRasterizerState(ID3D11RasterizerState* pRasterizerState)
	{
		mContext->RSSetState(pRasterizerState);
	}
	void CGraphicDevice_DX11::BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState)
	{
		mContext->OMSetDepthStencilState(pDepthStencilState, 0);
	}
	void CGraphicDevice_DX11::BindBlendState(ID3D11BlendState* pBlendState)
	{
		mContext->OMSetBlendState(pBlendState, nullptr, 0xffffffff);
	}
	void CGraphicDevice_DX11::Clear()
	{
		FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetTexture->GetRTV().Get(), backgroundColor);
		mContext->ClearDepthStencilView(mDepthStencilBufferTexture->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
	void CGraphicDevice_DX11::AdjustViewPorts()
	{
		// ViewPort, RenderTaget
		RECT winRect;
		GetClientRect(Application.GetHwnd(), &winRect);
		mViewPort = { 0.0f, 0.0f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.0f, 1.0f };
		BindViewPorts(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetTexture->GetRTV().GetAddressOf(), mDepthStencilBufferTexture->GetDSV().Get());
	}
	void CGraphicDevice_DX11::Draw()
	{
		mContext->Draw(0, 0);
	}
	void CGraphicDevice_DX11::DrawIndexed(UINT indexCount, UINT StartIndexLocation, UINT BaseVertexLocation)
	{
		mContext->DrawIndexed(indexCount, StartIndexLocation, BaseVertexLocation);
	}
	void CGraphicDevice_DX11::DrawIndexedInstanced(UINT IndexCountPerInstance,UINT InstanceCount,UINT StartIndexLocation,INT BaseVertexLocation, UINT StartInstanceLocation)
	{
		mContext->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
	}
	void CGraphicDevice_DX11::Present()
	{
		mSwapChain->Present(0, 0);
	}
}

		// ���� ���̵����
		
		// ���ҽ� ���ε�
		//D3D11_MAPPED_SUBRESOURCE sub = {};
		//mContext->Map(Renderer::triangleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		//memcpy(sub.pData, Renderer::vertexes, sizeof(Renderer::Vertex) * 4);
		
		// ����� �ʱ�ȭ
		//mContext->Unmap(Renderer::triangleBuffer, 0);

		// ȭ�� �����ֱ�
		//FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		//mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
		//mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		// ��� ���۸� ���̴��� ����
		//SetConstantBuffer(eShaderStage::VS, eCBType::Transform, Renderer::triangleConstantBuffer);

		// ViewPort, RenderTarget
		//RECT winRect;
		//GetClientRect(Application.GetHwnd(), &winRect);
		//mViewPort = { 0.0f, 0.0f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.0f, 1.0f };
		//BindViewPorts(&mViewPort);
		//mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

		// Input AssemBeler �ܰ迡 ���ؽ� ���� ���� ����
		//UINT vertexSIze = sizeof(Renderer::Vertex);
		//UINT offset = 0;
		//mContext->IASetVertexBuffers(0, 1, &Renderer::triangleBuffer, &vertexSIze, &offset);
		//mContext->IASetIndexBuffer(Renderer::triangleIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//mContext->IASetInputLayout(Renderer::triangleLayout);
		
		// ��� �׸��������� ���� ������
		//mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// ������ ���̴� ����
		//mContext->VSSetShader(Renderer::triangleVS,0 ,0);
		//mContext->PSSetShader(Renderer::trianglePS, 0, 0);

		//mContext->DrawIndexed(6, 0, 0);

		// ����ۿ� �׷��ش�
		//mSwapChain->Present(0, 0);


	//// 1. ȭ�� �ʱ�ȭ
		//Clear();

		//// 2. ������۸� ���̴��� ����
		//SetConstantBuffer(eShaderStage::VS, eCBType::Transform, Renderer::triangleConstantBuffer.Get());

		//// 3. ����Ʈ ����
		//AdjustViewPorts();

		//// 4. ���� ����
		//Renderer::mesh->BindBuffer();

		//// 5. ��ǲ���̾ƿ�, �������� ���� ( �׸��� ��� )
		//mContext->IASetInputLayout(Renderer::triangleLayout.Get());
		//mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//// 6. ������ ���̴� ����
		//mContext->VSSetShader(Renderer::triangleVS.Get(), 0, 0);
		//mContext->PSSetShader(Renderer::trianglePS.Get(), 0, 0);

		//// 7. �׸���
		//Renderer::mesh->Render();

		//// 8. ����ü��
		//Present();