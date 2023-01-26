#include "CGraphicDevice_DX11.h"
#include "CApplication.h"
#include "CRenderer.h"

extern CApplication Application;

namespace graphics
{
	CGraphicDevice_DX11::CGraphicDevice_DX11(ValidationMode validationMode)
	{
		// 1. Device 와 SwapChain 을 생성한다
		// 2. 백버퍼에 실제로 렌더링 할 렌더타겟 뷰를 생성해야 한다.
		// 3. 화면을 클리어 해줘야한다. 뷰포트를 생성해줘야 한다.
		// 4. 매프레임마다 위에서 생성한 렌더타겟뷰에 렌더링해주어야한다.
		// 5. SwapChain 을 이용하여 최종 디바이스(디스플레이)에 화면을 그려줘야 한다.

		HWND hwnd = Application.GetHwnd();

		// Device, Device Context
		UINT DeviceFlag = D3D11_CREATE_DEVICE_DEBUG; // 기본 모드 키값
		D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0; // 기본 모드

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

		// 디바이스 생성부
		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
										, DeviceFlag, nullptr, 0
										, D3D11_SDK_VERSION
										, mDevice.GetAddressOf()
										, &FeatureLevel
										, mContext.GetAddressOf());

		// 스왑체인 생성
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		swapChainDesc.OutputWindow = hwnd;												 // 핸들 값
		swapChainDesc.Windowed = true;													 // 창모드 여부
		swapChainDesc.BufferCount = 2;													 // 버퍼의 갯수 (기본적인 렌더는 두장)
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
		swapChainDesc.BufferDesc.Width = Application.GetWidth();
		swapChainDesc.BufferDesc.Height = Application.GetHeight();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;		  // 32 비트 모드
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 144;							  // 주사율 최대값
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;							  // 주사율 최솟값
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				  // 배율 값 조정
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 레스터라이즈 순서 지정

		swapChainDesc.SampleDesc.Count = 1;												  // 샘플링
		swapChainDesc.SampleDesc.Quality = 0;											  // 퀄리티 기본

		if (!CreateSwapChain(&swapChainDesc))											  // 스왑체인 생성
			return;

		// Get Render Target for SwapChain
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)mRenderTarget.GetAddressOf());

		// Get Render Target View
		hr = mDevice->CreateRenderTargetView(mRenderTarget.Get(), nullptr, mRenderTargetView.GetAddressOf());

		D3D11_TEXTURE2D_DESC depthBuffer = {};
		depthBuffer.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthBuffer.Usage = D3D11_USAGE_DEFAULT;										  // Default 값
		depthBuffer.CPUAccessFlags = 0;													  // CPU 가 필요한 상황에서 엑세스

		depthBuffer.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;				  // 알파채널을 지원하는 8비트 4개 총 32비트
		depthBuffer.Width = Application.GetWidth();										  
		depthBuffer.Height = Application.GetHeight();
		depthBuffer.ArraySize = 1;			
		// 버퍼 갯수
		depthBuffer.SampleDesc.Count = 1;
		depthBuffer.SampleDesc.Quality = 0;

		depthBuffer.MipLevels = 0;														  // 사이즈별로 연산을 줄이기위해 미리 몇장의 여분을 더 그려둠
		depthBuffer.MiscFlags = 0;

		if (!CreateTextur(&depthBuffer, mDepthStencilBuffer.GetAddressOf()))   // DepthStencilBuffer 생성
			return;

		if (FAILED(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf())))
			return;

		RECT winRect;
		GetClientRect(Application.GetHwnd(), &winRect);
		mViewPort = { 0.0f, 0.0f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.0f, 1.0f };
		BindViewPorts(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
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

		// 스왑체인 생성
		if (FAILED(pDXGIFactory->CreateSwapChain(mDevice.Get(), desc, mSwapChain.GetAddressOf())))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateTextur(D3D11_TEXTURE2D_DESC* desc, ID3D11Texture2D** ppTexture2D)
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
		// GPU에 보낼 데이터를 버퍼라고도 함
		if (FAILED(mDevice->CreateBuffer(desc, data, buffer)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
	{
		if (FAILED(mDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppVertexShader)
	{
		if (FAILED(mDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader)))
			return false;

		return true;
	}
	bool CGraphicDevice_DX11::CreateShader()
	{
		//ID3DBlob* errorBlob = nullptr;

		//std::filesystem::path shaderPath = std::filesystem::current_path().parent_path();
		//shaderPath += "\\Shader_Source\\";

		//// Vertex Shader
		//std::wstring vsPath(shaderPath.c_str());
		//vsPath += L"TriangleVS.hlsl";
		//D3DCompileFromFile(vsPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		//, "VS_Test", "vs_5_0", 0, 0, &Renderer::triangleVSBlob, &errorBlob);

		//mDevice->CreateVertexShader(Renderer::triangleVSBlob->GetBufferPointer()
		//							, Renderer::triangleVSBlob->GetBufferSize()
		//							, nullptr, &Renderer::triangleVS);

		//if (errorBlob)
		//{
		//	OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		//	errorBlob->Release();
		//	errorBlob = nullptr;
		//}

		//// PixelShader
		//std::wstring psPath(shaderPath.c_str());
		//psPath += L"TrianglePS.hlsl";
		//D3DCompileFromFile(psPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		//	, "PS_Test", "ps_5_0", 0, 0, &Renderer::trianglePSBlob, &errorBlob);

		//if (errorBlob)
		//{
		//	OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		//	errorBlob->Release();
		//	errorBlob = nullptr;
		//}

		//mDevice->CreatePixelShader(Renderer::trianglePSBlob->GetBufferPointer()
		//	, Renderer::trianglePSBlob->GetBufferSize()
		//	, nullptr, &Renderer::trianglePS);

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
	void CGraphicDevice_DX11::BindPixelShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
	}
	void CGraphicDevice_DX11::BindViewPorts(D3D11_VIEWPORT* viewPort)
	{
		mContext->RSSetViewports(1, viewPort);
	}
	void CGraphicDevice_DX11::BindConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size)
	{
		D3D11_MAPPED_SUBRESOURCE sub = {};
		mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, data, size);
		mContext->Unmap(buffer, 0);
	}
	void CGraphicDevice_DX11::SetConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
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
	void CGraphicDevice_DX11::Clear()
	{
		FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
		mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}
	void CGraphicDevice_DX11::AdjustViewPorts()
	{
		// ViewPort, RenderTaget
		RECT winRect;
		GetClientRect(Application.GetHwnd(), &winRect);
		mViewPort = { 0.0f, 0.0f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.0f, 1.0f };
		BindViewPorts(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	}
	void CGraphicDevice_DX11::Draw()
	{
		mContext->Draw(0, 0);
	}
	void CGraphicDevice_DX11::DrawIndexed(UINT indexCount, UINT StartIndexLocation, UINT BaseVertexLocation)
	{
		mContext->DrawIndexed(indexCount, StartIndexLocation, BaseVertexLocation);
	}
	void CGraphicDevice_DX11::Present()
	{
		mSwapChain->Present(0, 0);
	}
	void CGraphicDevice_DX11::Render()
	{
		// 1. 화면 초기화
		Clear();

		// 2. 상수버퍼를 셰이더에 세팅
		SetConstantBuffer(eShaderStage::VS, eCBType::Transform, Renderer::triangleConstantBuffer.Get());

		// 3. 뷰포트 조정
		AdjustViewPorts();

		// 4. 버퍼 세팅
		Renderer::mesh->BindBuffer();

		Renderer::shader->Binds();

		// 7. 그리기
		Renderer::mesh->Render();

		// 8. 스왑체인
		Present();
	}
}

		// 렌더 가이드라인
		
		// 리소스 바인딩
		//D3D11_MAPPED_SUBRESOURCE sub = {};
		//mContext->Map(Renderer::triangleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		//memcpy(sub.pData, Renderer::vertexes, sizeof(Renderer::Vertex) * 4);
		
		// 사용후 초기화
		//mContext->Unmap(Renderer::triangleBuffer, 0);

		// 화면 지워주기
		//FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		//mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
		//mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		// 상수 버퍼를 셰이더에 세팅
		//SetConstantBuffer(eShaderStage::VS, eCBType::Transform, Renderer::triangleConstantBuffer);

		// ViewPort, RenderTarget
		//RECT winRect;
		//GetClientRect(Application.GetHwnd(), &winRect);
		//mViewPort = { 0.0f, 0.0f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.0f, 1.0f };
		//BindViewPorts(&mViewPort);
		//mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

		// Input AssemBeler 단계에 버텍스 버퍼 정보 저장
		//UINT vertexSIze = sizeof(Renderer::Vertex);
		//UINT offset = 0;
		//mContext->IASetVertexBuffers(0, 1, &Renderer::triangleBuffer, &vertexSIze, &offset);
		//mContext->IASetIndexBuffer(Renderer::triangleIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//mContext->IASetInputLayout(Renderer::triangleLayout);
		
		// 어떻게 그릴지에대한 도형 연결방법
		//mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// 생성한 셰이더 세팅
		//mContext->VSSetShader(Renderer::triangleVS,0 ,0);
		//mContext->PSSetShader(Renderer::trianglePS, 0, 0);

		//mContext->DrawIndexed(6, 0, 0);

		// 백버퍼에 그려준다
		//mSwapChain->Present(0, 0);


	//// 1. 화면 초기화
		//Clear();

		//// 2. 상수버퍼를 셰이더에 세팅
		//SetConstantBuffer(eShaderStage::VS, eCBType::Transform, Renderer::triangleConstantBuffer.Get());

		//// 3. 뷰포트 조정
		//AdjustViewPorts();

		//// 4. 버퍼 세팅
		//Renderer::mesh->BindBuffer();

		//// 5. 인풋레이아웃, 토폴로지 세팅 ( 그리기 방식 )
		//mContext->IASetInputLayout(Renderer::triangleLayout.Get());
		//mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//// 6. 생성한 셰이더 세팅
		//mContext->VSSetShader(Renderer::triangleVS.Get(), 0, 0);
		//mContext->PSSetShader(Renderer::trianglePS.Get(), 0, 0);

		//// 7. 그리기
		//Renderer::mesh->Render();

		//// 8. 스왑체인
		//Present();