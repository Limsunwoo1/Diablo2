#include "CRenderer.h"
#include "CResourceManager.h"
#include "CMaterial.h"

namespace Renderer
{
	Vertex vertexes[4] = {};
	ConstantBuffer* constantBuffers[(UINT)eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};

	void SetUpState()
	{
		// Input Layout ( 정점 구조 정보)
		// (위치값, 칼라) 정보로 2개 생성
		D3D11_INPUT_ELEMENT_DESC arrLayoutDesc[3] = {}; // 구조체 항상 { } 초기화

		// 메모리 시작위치 오프셋
		arrLayoutDesc[0].AlignedByteOffset = 0;
		// 메모리 크기
		arrLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		// 구조 정보의 순서? 인풋 순서?
		arrLayoutDesc[0].InputSlot = 0;
		arrLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		// 시멘틱 네임
		arrLayoutDesc[0].SemanticName = "POSITION";
		arrLayoutDesc[0].SemanticIndex = 0;

		arrLayoutDesc[1].AlignedByteOffset = 16;
		arrLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayoutDesc[1].InputSlot = 0;
		arrLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[1].SemanticName = "COLOR";
		arrLayoutDesc[1].SemanticIndex = 0;

		arrLayoutDesc[2].AlignedByteOffset = 32;
		arrLayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayoutDesc[2].InputSlot = 0;
		arrLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[2].SemanticName = "TEXCOORD";
		arrLayoutDesc[2].SemanticIndex = 0;

		std::shared_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"RectShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> spriteshader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, spriteshader->GetVSBlobBufferPointer()
			, spriteshader->GetVSBlobBufferSize()
			, spriteshader->GetInputLayoutAddressOf());

		// 샘플러추가
		// Sampler State

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		//D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
		//D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;

		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, samplerStates[(UINT)eSamplerType::Point].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf()
		);

		GetDevice()->BindsSamplers((UINT)eSamplerType::Point
			, 1, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());

		GetDevice()->BindsSamplers((UINT)eSamplerType::Linear
			, 1, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf());

		GetDevice()->BindsSamplers((UINT)eSamplerType::Anisotropic
			, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
	}

	void LoadBuffer()
	{
		// Create Mesh
		std::shared_ptr<Mesh>mesh = std::make_shared<Mesh>();
		ResourceManager::GetInstance()->Insert<Mesh>(L"RectMesh", mesh);

		mesh->CreateVertexBuffer(vertexes, 4);

		std::vector<UINT> indexs;
		indexs.push_back(0);
		indexs.push_back(1);
		indexs.push_back(2);

		indexs.push_back(0);
		indexs.push_back(2);
		indexs.push_back(3);
		mesh->CreateIndexBuffer(indexs.data(), (UINT)indexs.size());

		constantBuffers[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		constantBuffers[(UINT)eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffers[(UINT)eCBType::Material]->Create(sizeof(MaterialCB));
	}

	void LoadShader()
	{
		// Dafault
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"VS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"PS.hlsl", "main");

		ResourceManager::GetInstance()->Insert<Shader>(L"RectShader", shader);

		// Sprite
		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		spriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		spriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");

		ResourceManager::GetInstance()->Insert<Shader>(L"SpriteShader", spriteShader);
	}

	void LoadMaterial()
	{
		std::shared_ptr<Texture> texture = ResourceManager::GetInstance()->Load<Texture>(L"SmileTexture", L"Smile.png");

		// Dafault
		std::shared_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"RectShader");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(texture);

		ResourceManager::GetInstance()->Insert<Material>(L"RectMaterial", material);

		std::shared_ptr <Texture> spriteTexture = ResourceManager::GetInstance()->Load<Texture>(L"DefaultSprite", L"DefaultSprite.png");
		// Sprite
		std::shared_ptr<Shader> spriteShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
		spriteMaterial->SetShader(spriteShader);
		spriteMaterial->SetTexture(spriteTexture);
		ResourceManager::GetInstance()->Insert<Material>(L"SpriteMaterial", spriteMaterial);
	}

	void Initialize()
	{
		//RECT
		vertexes[0].pos = Vector4(-0.5f, 0.5f, 0.5f, 1.0f);
		vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);
		vertexes[0].uv = Vector2(0.f, 0.f);

		vertexes[1].pos = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		vertexes[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		vertexes[1].uv = Vector2(1.f, 0.f);

		vertexes[2].pos = Vector4(0.5f, -0.5f, 0.5f, 1.0f);
		vertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
		vertexes[2].uv = Vector2(1.f, 1.f);

		vertexes[3].pos = Vector4(-0.5f, -0.5f, 0.5f, 1.0f);
		vertexes[3].color = Vector4(0.f, 0.f, 1.f, 1.f);
		vertexes[3].uv = Vector2(0.f, 1.f);

		LoadShader();
		SetUpState();
		LoadBuffer();
		LoadMaterial();
	}

	void Release()
	{
		for (size_t i = 0; i < (UINT)eCBType::End; ++i)
		{
			delete constantBuffers[i];
			constantBuffers[i] = nullptr;
		}
	}
}
