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
		// GPU ��ü ���� �׷���ī��� ����Ǵ� �⺻���ΰ�ü
		Microsoft::WRL::ComPtr <ID3D11Device> mDevice;
		// GPU read write ����̽��� ���� �������� �ʰ� �� ��ü�� ���� GPU�� ����� ������
		Microsoft::WRL::ComPtr <ID3D11DeviceContext> mContext;

		// ���������� �׷����� ��ȭ��
		Microsoft::WRL::ComPtr <ID3D11Texture2D> mRenderTarget;
		// ����Ÿ�� ��ü�� �����ϱ����� ��ü (��������X View�� ���� ��ȸ����)
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> mRenderTargetView; // ���� Ÿ�� ���� ���� - iter ���� ����

		Microsoft::WRL::ComPtr <ID3D11Texture2D> mDepthStencilBuffer;
		// ���������� �K ���ٽ� ���ۿ� �����ϱ����� ��ü
		Microsoft::WRL::ComPtr <ID3D11DepthStencilView> mDepthStencilView;

		// ȭ�鿡 ���������� �׷�����
		// ���۹�(Frame Buffer)�� �����ϰ�, ������ ȭ�鿡 ������ �ϴ�
		// ������ ����ϴ� ��ü
		Microsoft::WRL::ComPtr <IDXGISwapChain> mSwapChain;

		// �ؽ��� �ε��Ҷ� ���ȴ�.
		// ��Ƽ������� (������� ����)
		//ID3D11SamplerState* mSampler;

		D3D11_VIEWPORT mViewPort;
	};

	inline CGraphicDevice_DX11*& GetDevice()
	{
		static CGraphicDevice_DX11* device = nullptr;
		return device;
	}
}

