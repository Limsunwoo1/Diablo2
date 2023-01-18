#include "CRenderer.h"
#include "CResourceManager.h"

namespace Renderer
{
	// 정점 데이터
	Vertex vertexes[4] = {};

	// 버퍼
	Mesh* mesh = nullptr;
	// 상수 버퍼
	Microsoft::WRL::ComPtr < ID3D11Buffer> triangleConstantBuffer = nullptr;
	// 버텍스 셰이더
	Microsoft::WRL::ComPtr < ID3DBlob> triangleVSBlob = nullptr;
	Microsoft::WRL::ComPtr < ID3D11VertexShader> triangleVS = nullptr;

	// 픽셀 셰이더
	Microsoft::WRL::ComPtr < ID3DBlob> trianglePSBlob = nullptr;
	Microsoft::WRL::ComPtr < ID3D11PixelShader> trianglePS = nullptr;

	// 인풋 레이아웃 ( 정점 정보)
	Microsoft::WRL::ComPtr < ID3D11InputLayout> triangleLayout = nullptr;

	void SetUpState()
	{
		// Input Layout ( 정점 구조 정보)
		// (위치값, 칼라) 정보로 2개 생성
		D3D11_INPUT_ELEMENT_DESC arrLayoutDesc[2] = {}; // 구조체 항상 { } 초기화

		// 메모리 시작위치 오프셋
		arrLayoutDesc[0].AlignedByteOffset = 0;
		// 메모리 크기
		arrLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		// 구조 정보의 순서? 인풋 순서?
		arrLayoutDesc[0].InputSlot = 0;
		arrLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		// 시멘틱 네임
		arrLayoutDesc[0].SemanticName = "POSITION";
		arrLayoutDesc[0].SemanticIndex = 0;

		arrLayoutDesc[1].AlignedByteOffset = 12;
		arrLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayoutDesc[1].InputSlot = 0;
		arrLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[1].SemanticName = "COLOR";
		arrLayoutDesc[1].SemanticIndex = 0;

		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 2
			, triangleVSBlob->GetBufferPointer()
			, triangleVSBlob->GetBufferSize()
			, &triangleLayout);

	}

	void LoadBuffer()
	{
		// Create Mesh
		mesh = new Mesh();
		ResourceManager::GetInstance()->Insert<Mesh>(L"RectMesh", mesh);

		// 버텍스버퍼
		mesh->CreateVertexBuffer(vertexes, 4);

		// 인덱스 버퍼
		std::vector<UINT> indexs;
		indexs.push_back(0);
		indexs.push_back(1);
		indexs.push_back(2);

		indexs.push_back(0);
		indexs.push_back(2);
		indexs.push_back(3);

		mesh->CreateIndexBuffer(indexs.data(), indexs.size());

		// 상수 버퍼
		D3D11_BUFFER_DESC csDesc = {};
		csDesc.ByteWidth = sizeof(Vector4);
		csDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		csDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		csDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

		graphics::GetDevice()->CreateBuffer(&csDesc, nullptr, &triangleConstantBuffer);

		Vector4 pos(0.2f, 0.2f, 0.0f, 0.0f);
		graphics::GetDevice()->BindConstantBuffer(triangleConstantBuffer.Get(), &pos, sizeof(Vector4));
	}

	void LoadShader()
	{
		graphics::GetDevice()->CreateShader();
	}

	void Initialize()
	{
		//RECT
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.5f);
		vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.5f);
		vertexes[1].color = Vector4(1.f, 1.f, 1.f, 1.f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.5f);
		vertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.5f);
		vertexes[3].color = Vector4(0.f, 0.f, 1.f, 1.f);

		LoadShader();
		SetUpState();
		LoadBuffer();
	}

	void Release()
	{
		// 버퍼
		//triangleBuffer->Release();
		//triangleIndexBuffer->Release();
		//triangleConstantBuffer->Release();

		// VS
		//triangleVSBlob->Release();
		//triangleVS->Release();

		// PS
		//trianglePSBlob->Release();
		//trianglePS->Release();

		// Input Layout
		//triangleLayout->Release();
	}
}
