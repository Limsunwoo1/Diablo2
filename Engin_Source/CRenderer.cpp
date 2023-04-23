#include "CRenderer.h"
#include "CResourceManager.h"
#include "CMaterial.h"
#include "CSceneManager.h"
#include "CLight.h"
#include "CpaintShader.h"
#include "CTime.h"
#include "CParticleShader.h"
#include "CCamera.h"

namespace Renderer
{
	Vertex vertexes[4] = {};
	Vertex FadeInOut[4] = {};
	Vertex HalfAlpha[4] = {};
	ConstantBuffer* constantBuffers[(UINT)eCBType::End] = {};
	ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};
	ComPtr<ID3D11RasterizerState> RasterizeState[(UINT)eRasterizeType::End] = {};
	ComPtr<ID3D11DepthStencilState> Depth_StencilState[(UINT)eDepthStencilType::End] = {};
	ComPtr<ID3D11BlendState> BlendState[(UINT)eBlendType::End] = {};

	Camera* mainCamera = nullptr;
	Camera* UiCamera = nullptr;
	std::vector<Camera*> Cameras[(UINT)eSceneType::End];
	std::vector<DebugMesh> debugMeshes;
	std::vector<LightAttribute> lights;
	StructedBuffer* LightBuffer;

	float Time = 0.0f;

	void LoadMesh()
	{
#pragma region POINT MESH
		//Point Mesh
		Vertex v = {};
		shared_ptr<Mesh> pointMesh = make_shared<Mesh>();
		ResourceManager::GetInstance()->Insert(L"PointMesh", pointMesh);
		pointMesh->CreateVertexBuffer(&v, 1);

		UINT pointIndex = 0;
		pointMesh->CreateIndexBuffer(&pointIndex, 1);

#pragma endregion
#pragma region INDEX
		std::vector<UINT> indexs;
		indexs.push_back(0);
		indexs.push_back(1);
		indexs.push_back(2);

		indexs.push_back(0);
		indexs.push_back(2);
		indexs.push_back(3);
		indexs.push_back(0);
#pragma endregion
#pragma region RECT MESH
		//RECT
		vertexes[0].pos = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);
		vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);
		vertexes[0].uv = Vector2(0.f, 0.f);

		vertexes[1].pos = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
		vertexes[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		vertexes[1].uv = Vector2(1.f, 0.f);

		vertexes[2].pos = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
		vertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
		vertexes[2].uv = Vector2(1.f, 1.f);

		vertexes[3].pos = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
		vertexes[3].color = Vector4(0.f, 0.f, 1.f, 1.f);
		vertexes[3].uv = Vector2(0.f, 1.f);

		// Create Mesh
		std::shared_ptr<Mesh>mesh = std::make_shared<Mesh>();
		ResourceManager::GetInstance()->Insert<Mesh>(L"RectMesh", mesh);
		mesh->CreateVertexBuffer(vertexes, 4);
		mesh->CreateIndexBuffer(indexs.data(), (UINT)indexs.size());
#pragma endregion
#pragma region Half Alpha Mesh
		//RECT
		HalfAlpha[0].pos = Vector4(-0.5f, 0.5f, 0.0f, 1.0f);
		HalfAlpha[0].color = Vector4(0.f, 1.f, 0.f, 0.7f);
		HalfAlpha[0].uv = Vector2(0.f, 0.f);

		HalfAlpha[1].pos = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
		HalfAlpha[1].color = Vector4(1.f, 1.f, 1.f, 0.7f);
		HalfAlpha[1].uv = Vector2(1.f, 0.f);

		HalfAlpha[2].pos = Vector4(0.5f, -0.5f, 0.0f, 1.0f);
		HalfAlpha[2].color = Vector4(1.f, 0.f, 0.f, 0.7f);
		HalfAlpha[2].uv = Vector2(1.f, 1.f);

		HalfAlpha[3].pos = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
		HalfAlpha[3].color = Vector4(0.f, 0.f, 1.f, 0.7f);
		HalfAlpha[3].uv = Vector2(0.f, 1.f);

		// Create Mesh
		std::shared_ptr<Mesh>halfmesh = std::make_shared<Mesh>();
		ResourceManager::GetInstance()->Insert<Mesh>(L"HalfAlphaMesh", halfmesh);
		halfmesh->CreateVertexBuffer(HalfAlpha, 4);
		halfmesh->CreateIndexBuffer(indexs.data(), (UINT)indexs.size());
#pragma endregion

#pragma region FADE MESH
		// Fade
		FadeInOut[0].pos = Vector4(-1.0f, 1.0f, 0.0f, 1.0f);
		FadeInOut[0].color = Vector4(0.f, 0.f, 0.f, 0.f);
		FadeInOut[0].uv = Vector2(0.f, 0.f);

		FadeInOut[1].pos = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		FadeInOut[1].color = Vector4(0.f, 0.f, 0.f, 1.f);
		FadeInOut[1].uv = Vector2(1.f, 0.f);

		FadeInOut[2].pos = Vector4(1.0f, -1.0f, 0.0f, 1.0f);
		FadeInOut[2].color = Vector4(0.f, 0.f, 0.f, 1.f);
		FadeInOut[2].uv = Vector2(1.f, 1.f);

		FadeInOut[3].pos = Vector4(-1.0f, -1.0f, 0.0f, 1.0f);
		FadeInOut[3].color = Vector4(0.f, 0.f, 0.f, 1.f);
		FadeInOut[3].uv = Vector2(0.f, 1.f);


		std::shared_ptr<Mesh>Fademesh = std::make_shared<Mesh>();
		ResourceManager::GetInstance()->Insert<Mesh>(L"FadeMesh", Fademesh);

		Fademesh->CreateVertexBuffer(FadeInOut, 4);
		Fademesh->CreateIndexBuffer(indexs.data(), (UINT)indexs.size());
#pragma endregion
#pragma region DEBUG MESH
		// Debug
		vertexes[0].pos = Vector4(-0.5f, 0.5f, -0.00001f, 1.0f);
		vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);
		vertexes[0].uv = Vector2(0.f, 0.f);

		vertexes[1].pos = Vector4(0.5f, 0.5f, -0.00001f, 1.0f);
		vertexes[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		vertexes[1].uv = Vector2(1.f, 0.f);

		vertexes[2].pos = Vector4(0.5f, -0.5f, -0.00001f, 1.0f);
		vertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
		vertexes[2].uv = Vector2(1.f, 1.f);

		vertexes[3].pos = Vector4(-0.5f, -0.5f, -0.00001f, 1.0f);
		vertexes[3].color = Vector4(0.f, 0.f, 1.f, 1.f);
		vertexes[3].uv = Vector2(0.f, 1.f);

		std::shared_ptr<Mesh> DebugMesh = std::make_shared<Mesh>();
		ResourceManager::GetInstance()->Insert<Mesh>(L"DebugRectMesh", DebugMesh);
		DebugMesh->CreateVertexBuffer(vertexes, 4);

		std::vector<int> debugIndex;
		debugIndex.push_back(0);
		debugIndex.push_back(1);
		debugIndex.push_back(2);
		debugIndex.push_back(3);
		debugIndex.push_back(0);

		DebugMesh->CreateIndexBuffer(debugIndex.data(), debugIndex.size());
#pragma endregion
#pragma region CIRCLE MESH
		// Circle Mesh
		std::vector<Vertex> circleVertex;
		Vertex center = {};
		center.pos = Vector4(0.0f, 0.0f, -0.00001f, 1.0f);
		center.color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		center.uv = Vector2::Zero;

		circleVertex.push_back(center);

		int iSlice = 80;
		float fRadius = 0.5f;
		float fTheta = XM_2PI / (float)iSlice;

		for (int i = 0; i < iSlice; ++i)
		{
			Vertex vtx = {};
			vtx.pos = Vector4
			(
				fRadius * cosf(fTheta * (float)i)
				, fRadius * sinf(fTheta * (float)i)
				, -0.00001f, 1.0f
			);
			vtx.color = center.color;

			circleVertex.push_back(vtx);
		}
		indexs.clear();

		for (int i = 0; i < iSlice - 2; i++)
		{
			indexs.push_back(i + 1);
		}
		indexs.push_back(1);

		// Creat Mesh
		std::shared_ptr<Mesh> circleMesh = std::make_shared<Mesh>();
		ResourceManager::GetInstance()->Insert<Mesh>(L"CircleMesh", circleMesh);
		circleMesh->CreateVertexBuffer(circleVertex.data(), circleVertex.size());
		circleMesh->CreateIndexBuffer(indexs.data(), indexs.size());
#pragma endregion
	}

	void SetUpState()
	{
#pragma region InputLayout
		// Input Layout ( 정점 구조 정보 )
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

		std::shared_ptr<Shader> uishader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, uishader->GetVSBlobBufferPointer()
			, uishader->GetVSBlobBufferSize()
			, uishader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> gridshader = ResourceManager::GetInstance()->Find<Shader>(L"GridShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, gridshader->GetVSBlobBufferPointer()
			, gridshader->GetVSBlobBufferSize()
			, gridshader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> fadeShader = ResourceManager::GetInstance()->Find<Shader>(L"FadeShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, fadeShader->GetVSBlobBufferPointer()
			, fadeShader->GetVSBlobBufferSize()
			, fadeShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> debugShader = ResourceManager::GetInstance()->Find<Shader>(L"DebugShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, debugShader->GetVSBlobBufferPointer()
			, debugShader->GetVSBlobBufferSize()
			, debugShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> particleShader = ResourceManager::GetInstance()->Find<Shader>(L"ParticleShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, particleShader->GetVSBlobBufferPointer()
			, particleShader->GetVSBlobBufferSize()
			, particleShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> pingShader = ResourceManager::GetInstance()->Find<Shader>(L"PingShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, pingShader->GetVSBlobBufferPointer()
			, pingShader->GetVSBlobBufferSize()
			, pingShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> RungaugeShader = ResourceManager::GetInstance()->Find<Shader>(L"GaugeShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, RungaugeShader->GetVSBlobBufferPointer()
			, RungaugeShader->GetVSBlobBufferSize()
			, RungaugeShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> ItemShader = ResourceManager::GetInstance()->Find<Shader>(L"ItemShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, ItemShader->GetVSBlobBufferPointer()
			, ItemShader->GetVSBlobBufferSize()
			, ItemShader->GetInputLayoutAddressOf());

		std::shared_ptr<Shader> ItemSlotShader = ResourceManager::GetInstance()->Find<Shader>(L"ItemSlotShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, ItemSlotShader->GetVSBlobBufferPointer()
			, ItemSlotShader->GetVSBlobBufferSize()
			, ItemSlotShader->GetInputLayoutAddressOf());

#pragma endregion
#pragma region Sampler State
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
#pragma endregion
#pragma region Rasterizer State
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

		GetDevice()->CreateRasterizerState(&rsDesc, RasterizeState[(UINT)eRasterizeType::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

		GetDevice()->CreateRasterizerState(&rsDesc, RasterizeState[(UINT)eRasterizeType::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc, RasterizeState[(UINT)eRasterizeType::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc, RasterizeState[(UINT)eRasterizeType::WirefameNone].GetAddressOf());
#pragma endregion
#pragma region DepthStencil State
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc, Depth_StencilState[(UINT)eDepthStencilType::Less].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc, Depth_StencilState[(UINT)eDepthStencilType::Greater].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc, Depth_StencilState[(UINT)eDepthStencilType::NoWrite].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc, Depth_StencilState[(UINT)eDepthStencilType::None].GetAddressOf());
#pragma endregion
#pragma region Blend State
		BlendState[(UINT)eBlendType::Default] = nullptr;

		D3D11_BLEND_DESC blDesc = {};
		blDesc.AlphaToCoverageEnable = false;
		blDesc.IndependentBlendEnable = false;
		blDesc.RenderTarget[0].BlendEnable = true;
		blDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		blDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&blDesc, BlendState[(UINT)eBlendType::AlphaBlend].GetAddressOf());

		blDesc.AlphaToCoverageEnable = false;
		blDesc.IndependentBlendEnable = false;

		blDesc.RenderTarget[0].BlendEnable = true;
		blDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&blDesc, BlendState[(UINT)eBlendType::OneOne].GetAddressOf());

#pragma endregion
	}

	void LoadBuffer()
	{
#pragma region CONSTANT BUFFER
		constantBuffers[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		constantBuffers[(UINT)eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffers[(UINT)eCBType::Material]->Create(sizeof(MaterialCB));

		constantBuffers[(UINT)eCBType::Grid] = new ConstantBuffer(eCBType::Grid);
		constantBuffers[(UINT)eCBType::Grid]->Create(sizeof(GridCB));

		constantBuffers[(UINT)eCBType::Fade] = new ConstantBuffer(eCBType::Fade);
		constantBuffers[(UINT)eCBType::Fade]->Create(sizeof(FadeCB));

		constantBuffers[(UINT)eCBType::Animation] = new ConstantBuffer(eCBType::Animation);
		constantBuffers[(UINT)eCBType::Animation]->Create(sizeof(AnimationCB));

		constantBuffers[(UINT)eCBType::Light] = new ConstantBuffer(eCBType::Light);
		constantBuffers[(UINT)eCBType::Light]->Create(sizeof(LightCB));

		constantBuffers[(UINT)eCBType::Time] = new ConstantBuffer(eCBType::Time);
		constantBuffers[(UINT)eCBType::Time]->Create(sizeof(TimeCB));

		constantBuffers[(UINT)eCBType::ParticleSystem] = new ConstantBuffer(eCBType::ParticleSystem);
		constantBuffers[(UINT)eCBType::ParticleSystem]->Create(sizeof(ParticleSystemCB));

		constantBuffers[(UINT)eCBType::Noise] = new ConstantBuffer(eCBType::Noise);
		constantBuffers[(UINT)eCBType::Noise]->Create(sizeof(NoiseCB));

		constantBuffers[(UINT)eCBType::PlayerData] = new ConstantBuffer(eCBType::PlayerData);
		constantBuffers[(UINT)eCBType::PlayerData]->Create(sizeof(PlayerDataCB));

		constantBuffers[(UINT)eCBType::ItemData] = new ConstantBuffer(eCBType::ItemData);
		constantBuffers[(UINT)eCBType::ItemData]->Create(sizeof(ItemDataCB));
#pragma endregion
#pragma region STRUCTED BUFER
		// Structed buffer
		LightBuffer = new StructedBuffer();
		LightBuffer->Create(sizeof(LightAttribute), 128, eSRVType::SRV, nullptr, true);
#pragma endregion
	}

	void LoadShader()
	{
#pragma region DEFAULT SHDAER
		// Dafault
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"VS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"PS.hlsl", "main");

		ResourceManager::GetInstance()->Insert<Shader>(L"RectShader", shader);
#pragma endregion
#pragma region SPRITE SHADER
		// Sprite
		std::shared_ptr<Shader> spriteShader = std::make_shared<Shader>();
		spriteShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		spriteShader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");
		spriteShader->SetRasterize(eRasterizeType::SolidNone);
		//spriteShader->SetDepthStencil(eDepthStencilType::NoWrite);
		spriteShader->SetBlend(eBlendType::AlphaBlend);

		ResourceManager::GetInstance()->Insert<Shader>(L"SpriteShader", spriteShader);
#pragma endregion
#pragma region Item SHADER
		// Item
		std::shared_ptr<Shader> ItemShader = std::make_shared<Shader>();
		ItemShader->Create(eShaderStage::VS, L"ItemVS.hlsl", "main");
		ItemShader->Create(eShaderStage::PS, L"ItemPS.hlsl", "main");
		ItemShader->SetRasterize(eRasterizeType::SolidNone);
		//spriteShader->SetDepthStencil(eDepthStencilType::NoWrite);
		ItemShader->SetBlend(eBlendType::AlphaBlend);

		ResourceManager::GetInstance()->Insert<Shader>(L"ItemShader", ItemShader);
#pragma endregion
#pragma region ItemSlot SHADER
		// Item
		std::shared_ptr<Shader> ItemSlotShader = std::make_shared<Shader>();
		ItemSlotShader->Create(eShaderStage::VS, L"ItemVS.hlsl", "main");
		ItemSlotShader->Create(eShaderStage::PS, L"ItemSlotPS.hlsl", "main");
		ItemSlotShader->SetRasterize(eRasterizeType::SolidNone);
		//ItemSlotShader->SetDepthStencil(eDepthStencilType::NoWrite);
		ItemSlotShader->SetBlend(eBlendType::AlphaBlend);

		ResourceManager::GetInstance()->Insert<Shader>(L"ItemSlotShader", ItemSlotShader);
#pragma endregion
#pragma region UI SHADER
		// UI
		std::shared_ptr<Shader> uiShader = std::make_shared<Shader>();
		uiShader->Create(eShaderStage::VS, L"UserInterfaceVS.hlsl", "main");
		uiShader->Create(eShaderStage::PS, L"UserInterfacePS.hlsl", "main");

		ResourceManager::GetInstance()->Insert<Shader>(L"UIShader", uiShader);
#pragma endregion
#pragma region Gauge SHADER
		// UI
		std::shared_ptr<Shader> RunGaugeShader = std::make_shared<Shader>();
		RunGaugeShader->Create(eShaderStage::VS, L"UserInterfaceVS.hlsl", "main");
		RunGaugeShader->Create(eShaderStage::PS, L"GaugePS.hlsl", "main");

		ResourceManager::GetInstance()->Insert<Shader>(L"GaugeShader", RunGaugeShader);
#pragma endregion
#pragma region GRID SHADER
		// Grid
		std::shared_ptr<Shader> gridShader = std::make_shared<Shader>();
		gridShader->Create(eShaderStage::VS, L"GridVS.hlsl", "main");
		gridShader->Create(eShaderStage::PS, L"GridPS.hlsl", "main");
		gridShader->SetRasterize(eRasterizeType::SolidNone);
		gridShader->SetDepthStencil(eDepthStencilType::NoWrite);
		gridShader->SetBlend(eBlendType::AlphaBlend);

		ResourceManager::GetInstance()->Insert<Shader>(L"GridShader", gridShader);
#pragma endregion
#pragma region FADE SHADER
		// FadeInOut
		std::shared_ptr<Shader> fadeShader = std::make_shared<Shader>();
		fadeShader->Create(eShaderStage::VS, L"FadeInOutVS.hlsl", "main");
		fadeShader->Create(eShaderStage::PS, L"FadeInOutPS.hlsl", "main");
		fadeShader->SetRasterize(eRasterizeType::SolidNone);
		fadeShader->SetDepthStencil(eDepthStencilType::NoWrite);
		fadeShader->SetBlend(eBlendType::AlphaBlend);

		ResourceManager::GetInstance()->Insert<Shader>(L"FadeShader", fadeShader);
#pragma endregion
#pragma region DEBUG SHADER
		// Debug Shader
		std::shared_ptr<Shader> DebugShader = std::make_shared<Shader>();
		DebugShader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
		DebugShader->Create(eShaderStage::PS, L"DebugPS.hlsl", "main");

		DebugShader->SetRasterize(eRasterizeType::SolidNone);
		DebugShader->SetDepthStencil(eDepthStencilType::Less);
		DebugShader->SetBlend(eBlendType::AlphaBlend);

		DebugShader->SetToplogy(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		ResourceManager::GetInstance()->Insert<Shader>(L"DebugShader", DebugShader);
#pragma endregion
#pragma region PAINT SHADER
		// Paint Shader
		std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
		paintShader->Create(L"PaintCS.hlsl", "main");

		ResourceManager::GetInstance()->Insert<PaintShader>(L"PaintShader", paintShader);
#pragma endregion
#pragma region PARTICLE SHADER
		// Particle Shader
		std::shared_ptr<Shader> particleShader = std::make_shared<Shader>();
		particleShader->Create(eShaderStage::VS, L"ParticleVS.hlsl", "main");
		particleShader->Create(eShaderStage::GS, L"ParticleGS.hlsl", "main");
		particleShader->Create(eShaderStage::PS, L"ParticlePS.hlsl", "main");

		particleShader->SetRasterize(eRasterizeType::SolidNone);
		particleShader->SetDepthStencil(eDepthStencilType::NoWrite);
		particleShader->SetBlend(eBlendType::AlphaBlend);
		particleShader->SetToplogy(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		ResourceManager::GetInstance()->Insert<Shader>(L"ParticleShader", particleShader);

		// ParticleCS (ComputeSHader)
		std::shared_ptr<ParticleShader> particleCS = std::make_shared<ParticleShader>();
		particleCS->Create(L"ParticleCS.hlsl", "main");

		ResourceManager::GetInstance()->Insert<ParticleShader>(L"ParticleCS", particleCS);
#pragma endregion
#pragma region PingShader
		std::shared_ptr<Shader> pingshader = std::make_shared<Shader>();
		pingshader->Create(eShaderStage::VS, L"VS.hlsl", "main");
		pingshader->Create(eShaderStage::PS, L"PingPS.hlsl", "main");

		ResourceManager::GetInstance()->Insert<Shader>(L"PingShader", pingshader);
#pragma endregion

	}

	void LoadTexture()
	{
#pragma region LOAD
		ResourceManager::GetInstance()->Load<Texture2D>(L"SmileTexture", L"Smile.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"DefaultSprite", L"Light.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"HPBarTexture", L"HPBar.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"ShopIdle", L"ShopIdle.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"Ping", L"ping.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"noise_01", L"noise//noise_01.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"noise_02", L"noise//noise_02.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"Green", L"Green.png");
		//ResourceManager::GetInstance()->Load<Texture2D>(L"Diablo2_Town_Idle", L"diablo2_Town_Idle.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"CartoonSmoke", L"particle\\CartoonSmoke.png");
#pragma endregion
#pragma region DYNAMIC
		// Create
		std::shared_ptr<Texture2D> uavTexture = std::make_shared<Texture2D>();
		uavTexture->Create(1024, 1024, DXGI_FORMAT_B8G8R8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
		ResourceManager::GetInstance()->Insert<Texture2D>(L"PaintTexture", uavTexture);
#pragma endregion
		// CS 과제용 노이즈텍스쳐
		ResourceManager::GetInstance()->Load<Texture2D>(L"Noise", L"noise.png");
		std::shared_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"Noise");
		texture->BindShaderResource(graphics::eShaderStage::CS, 2);
		texture->BindShaderResource(graphics::eShaderStage::PS, 2);
	}

	void LoadMaterial()
	{
#pragma region DEFAULT MATERIAL
		// Dafault
		std::shared_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"SmileTexture");
		std::shared_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"RectShader");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		ResourceManager::GetInstance()->Insert<Material>(L"RectMaterial", material);
#pragma endregion
#pragma region PAINT MATERIAL
		{
			std::shared_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"PaintTexture");
			std::shared_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"PaintShader");
			std::shared_ptr<Material> material = std::make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(eTextureSlot::T0, texture);
			ResourceManager::GetInstance()->Insert<Material>(L"noiseMaterial", material);
		}
#pragma endregion
#pragma region SPTRITE MATERIAL
		std::shared_ptr <Texture2D> spriteTexture = ResourceManager::GetInstance()->Find<Texture2D>(L"DefaultSprite");
		std::shared_ptr<Shader> spriteShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
		spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
		spriteMaterial->SetShader(spriteShader);
		spriteMaterial->SetTexture(eTextureSlot::T0, spriteTexture);
		ResourceManager::GetInstance()->Insert<Material>(L"SpriteMaterial", spriteMaterial);
#pragma endregion
#pragma region UI MATERIAL
		// UI
		std::shared_ptr <Texture2D> uiTexture = ResourceManager::GetInstance()->Find<Texture2D>(L"HPBarTexture");
		std::shared_ptr<Shader> uiShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> uiMaterial = std::make_shared<Material>();
		uiMaterial->SetRenderingMode(eRenderingMode::Transparent);
		uiMaterial->SetShader(uiShader);
		uiMaterial->SetTexture(eTextureSlot::T0, uiTexture);
		ResourceManager::GetInstance()->Insert<Material>(L"UIMaterial", uiMaterial);
#pragma endregion
#pragma region HP MATERIAL
		// UI
		std::shared_ptr<Shader> HPShader = ResourceManager::GetInstance()->Find<Shader>(L"GaugeShader");
		std::shared_ptr<Material> HPMaterial = std::make_shared<Material>();
		HPMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		HPMaterial ->SetShader(HPShader);
		ResourceManager::GetInstance()->Insert<Material>(L"HPMaterial", HPMaterial);
#pragma endregion
#pragma region MP MATERIAL
		// UI
		std::shared_ptr<Shader> MPShader = ResourceManager::GetInstance()->Find<Shader>(L"GaugeShader");
		std::shared_ptr<Material> MPMaterial = std::make_shared<Material>();
		MPMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		MPMaterial ->SetShader(MPShader);
		ResourceManager::GetInstance()->Insert<Material>(L"MPMaterial", MPMaterial);
#pragma endregion
#pragma region RunGauge MATERIAL
		// UI
		std::shared_ptr<Shader> RungaugeShader = ResourceManager::GetInstance()->Find<Shader>(L"GaugeShader");
		std::shared_ptr<Material> RunGaugeMaterial = std::make_shared<Material>();
		RunGaugeMaterial->SetRenderingMode(eRenderingMode::Transparent);
		RunGaugeMaterial->SetShader(RungaugeShader);
		ResourceManager::GetInstance()->Insert<Material>(L"RunGaugeMaterial", RunGaugeMaterial);
#pragma endregion
#pragma region HP Panel MATERIAL
		// UI
		std::shared_ptr<Shader> hpPanelShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> hpPanelMaterial = std::make_shared<Material>();
		hpPanelMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		hpPanelMaterial ->SetShader(hpPanelShader);
		ResourceManager::GetInstance()->Insert<Material>(L"HpPanelMaterial", hpPanelMaterial);
#pragma endregion
#pragma region MP Panel MATERIAL
		// UI
		std::shared_ptr<Shader> mpShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> mpMaterial = std::make_shared<Material>();
		mpMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		mpMaterial ->SetShader(mpShader);
		ResourceManager::GetInstance()->Insert<Material>(L"MpPanelMaterial", mpMaterial);
#pragma endregion
#pragma region RunIcon MATERIAL
		// UI
		std::shared_ptr<Shader> RunIconShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> RunIconMaterial = std::make_shared<Material>();
		RunIconMaterial->SetRenderingMode(eRenderingMode::Transparent);
		RunIconMaterial->SetShader(RunIconShader);
		ResourceManager::GetInstance()->Insert<Material>(L"RunIconMaterial", RunIconMaterial);
#pragma endregion
#pragma region WalkIcon MATERIAL
		// UI
		std::shared_ptr<Shader> WalkIconShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> WalkIconMaterial = std::make_shared<Material>();
		WalkIconMaterial->SetRenderingMode(eRenderingMode::Transparent);
		WalkIconMaterial->SetShader(WalkIconShader);
		ResourceManager::GetInstance()->Insert<Material>(L"WalkIconMaterial", WalkIconMaterial);
#pragma endregion
#pragma region Main MATERIAL
		// UI
		std::shared_ptr<Shader> mainShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> mainuiMaterial = std::make_shared<Material>();
		mainuiMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		mainuiMaterial ->SetShader(mainShader);
		ResourceManager::GetInstance()->Insert<Material>(L"mainPanelMaterial", mainuiMaterial);
#pragma endregion
#pragma region Main MATERIAL
		// UI
		std::shared_ptr<Shader> InventoryShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> InventoryMaterial = std::make_shared<Material>();
		InventoryMaterial->SetRenderingMode(eRenderingMode::Transparent);
		InventoryMaterial->SetShader(InventoryShader);
		ResourceManager::GetInstance()->Insert<Material>(L"InventoryMaterial", InventoryMaterial);
#pragma endregion
#pragma region GRID MATERIAL
		// Grid
		std::shared_ptr<Shader> GridShader = ResourceManager::GetInstance()->Find<Shader>(L"GridShader");
		std::shared_ptr<Material> gridMaterial = std::make_shared<Material>();
		gridMaterial->SetShader(GridShader);
		ResourceManager::GetInstance()->Insert(L"GridMaterial", gridMaterial);
#pragma endregion
#pragma region FADE MATERIAL
		// FadeInOut
		std::shared_ptr<Shader> FadeShader = ResourceManager::GetInstance()->Find<Shader>(L"FadeShader");
		std::shared_ptr<Material> FadeMaterial = std::make_shared<Material>();
		FadeMaterial->SetShader(FadeShader);
		ResourceManager::GetInstance()->Insert(L"FadeMaterial", FadeMaterial);
#pragma endregion
#pragma region DEBUG MATERIAL
		// Debug
		std::shared_ptr<Shader> debugShader = ResourceManager::GetInstance()->Find<Shader>(L"DebugShader");
		std::shared_ptr<Material> debugMaterial = std::make_shared<Material>();
		debugMaterial->SetRenderingMode(eRenderingMode::Transparent);
		debugMaterial->SetShader(debugShader);
		ResourceManager::GetInstance()->Insert<Material>(L"DebugMaterial", debugMaterial);
#pragma endregion
#pragma region PARTICLE MATERIAL
		// Particle
		{
			std::shared_ptr<Shader> particleShader = ResourceManager::GetInstance()->Find<Shader>(L"ParticleShader");
			std::shared_ptr<Material> particleMaterial = std::make_shared<Material>();
			particleMaterial->SetRenderingMode(eRenderingMode::Transparent);
			particleMaterial->SetShader(particleShader);
			ResourceManager::GetInstance()->Insert<Material>(L"ParticleMaterial", particleMaterial);
		}
#pragma endregion
#pragma region PingMaterial
		std::shared_ptr<Shader> pingShdaer = ResourceManager::GetInstance()->Find<Shader>(L"PingShader");
		std::shared_ptr<Material> pingMaterial = std::make_shared<Material>();
		pingMaterial->SetRenderingMode(eRenderingMode::Transparent);
		pingMaterial->SetShader(pingShdaer);
		ResourceManager::GetInstance()->Insert<Material>(L"PingMaterial", pingMaterial);
#pragma endregion
#pragma region FrozenOrb
		std::shared_ptr<Shader> frozenShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> FrozenOrbMater = std::make_shared<Material>();
		FrozenOrbMater->SetRenderingMode(eRenderingMode::Transparent);
		FrozenOrbMater->SetShader(frozenShader);
		ResourceManager::GetInstance()->Insert<Material>(L"FrozenOrbMaterial", FrozenOrbMater);
#pragma endregion
#pragma region FrozenMisile
		std::shared_ptr<Shader> frozenMisileShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> FrozenMisile = std::make_shared<Material>();
		FrozenMisile->SetRenderingMode(eRenderingMode::Transparent);
		FrozenMisile->SetShader(frozenMisileShader);
		ResourceManager::GetInstance()->Insert<Material>(L"FrozenMisileMaterial", FrozenMisile);
#pragma endregion
#pragma region TelePortMaterail
		std::shared_ptr<Shader> TeleParteShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> TelePort = std::make_shared<Material>();
		TelePort->SetRenderingMode(eRenderingMode::Transparent);
		TelePort->SetShader(TeleParteShader);
		ResourceManager::GetInstance()->Insert<Material>(L"TelePortMaterial", TelePort);
#pragma endregion
#pragma region AStarRenderMaterial
		std::shared_ptr<Shader> AstarShader = ResourceManager::GetInstance()->Find<Shader>(L"FadeShader");
		std::shared_ptr<Texture2D> green = ResourceManager::GetInstance()->Find<Texture2D>(L"Green");
		std::shared_ptr<Material> AstarMater = std::make_shared<Material>();
		AstarMater->SetRenderingMode(eRenderingMode::Transparent);
		AstarMater->SetShader(AstarShader);
		AstarMater->SetTexture(eTextureSlot::T0 ,green);
		ResourceManager::GetInstance()->Insert<Material>(L"AstarMaterial", AstarMater);
#pragma endregion
#pragma region MeteorMaterial
		std::shared_ptr<Shader> MeteorShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> MeteorMaterial = std::make_shared<Material>();
		MeteorMaterial->SetRenderingMode(eRenderingMode::Transparent);
		MeteorMaterial->SetShader(MeteorShader);
		ResourceManager::GetInstance()->Insert<Material>(L"MeteorMaterial", MeteorMaterial);
#pragma endregion
#pragma region FalmeMaterial
		std::shared_ptr<Shader> flameShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> flameMaterial = std::make_shared<Material>();
		flameMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		flameMaterial ->SetShader(flameShader);
		ResourceManager::GetInstance()->Insert<Material>(L"FlameMaterial", flameMaterial);
#pragma endregion
#pragma region FirePinMaterial
		std::shared_ptr<Shader> PinShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> firtPinMaterial = std::make_shared<Material>();
		firtPinMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		firtPinMaterial ->SetShader(PinShader);
		ResourceManager::GetInstance()->Insert<Material>(L"FirePinMaterial", firtPinMaterial);
#pragma endregion
#pragma region MinoTauros
		std::shared_ptr<Shader> MinoShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> MinoMaterial = std::make_shared<Material>();
		MinoMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		MinoMaterial ->SetShader(MinoShader);
		ResourceManager::GetInstance()->Insert<Material>(L"MinoMaterial", MinoMaterial);
#pragma endregion
#pragma region ItemSlot
		std::shared_ptr<Shader> ItemSlotShader = ResourceManager::GetInstance()->Find<Shader>(L"ItemSlotShader");
		std::shared_ptr<Material> ItemSlotMaterial = std::make_shared<Material>();
		ItemSlotMaterial->SetRenderingMode(eRenderingMode::Transparent);
		ItemSlotMaterial->SetShader(ItemSlotShader);
		ResourceManager::GetInstance()->Insert<Material>(L"ItemSlotMaterial", ItemSlotMaterial);
#pragma endregion

	}

	void Initialize()
	{
		LoadShader();
		SetUpState();
		LoadMesh();
		LoadBuffer();
		LoadTexture();
		LoadMaterial();
	}

	void Render()
	{
		BindNoiseTexture();
		BindLights();
		//BindTime();

		eSceneType type = SceneManager::GetInstance()->GetActiveScene()->GetScenType();
		for (Camera* cam : Cameras[(UINT)type])
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		Cameras[(UINT)type].clear();
		lights.clear();
	}

	void Release()
	{
		for (size_t i = 0; i < (UINT)eCBType::End; ++i)
		{
			delete constantBuffers[i];
			constantBuffers[i] = nullptr;
		}

		delete LightBuffer;
		LightBuffer = nullptr;
	}

	void PushLightAttribute(LightAttribute lightAttribute)
	{
		lights.push_back(lightAttribute);
	}

	void BindLights()
	{
		LightBuffer->SetData(lights.data(), lights.size());
		LightBuffer->BindSRV(eShaderStage::VS, 13);
		LightBuffer->BindSRV(eShaderStage::PS, 13);

		LightCB trCb = {};
		trCb.numberOfLight = lights.size();

		ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Light];
		cb->SetData(&trCb);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	void BindTime()
	{
		Time += Time::GetInstance()->DeltaTime();
		if (Time >= 1.0f)
			Time -= 1.0f;

		TimeCB tCb = {};
		tCb.deltatime = Time::GetInstance()->DeltaTime();
		tCb.time = Time;

		ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Time];
		cb->SetData(&tCb);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}

	void BindNoiseTexture()
	{
		std::shared_ptr<Texture2D> noise = ResourceManager::GetInstance()->Find<Texture2D>(L"noise_01");
		noise->BindShaderResource(eShaderStage::VS, 16);
		noise->BindShaderResource(eShaderStage::HS, 16);
		noise->BindShaderResource(eShaderStage::DS, 16);
		noise->BindShaderResource(eShaderStage::GS, 16);
		noise->BindShaderResource(eShaderStage::PS, 16);
		noise->BindShaderResource(eShaderStage::CS, 16);
		noise->BindShaderResource(eShaderStage::VS, 16);

		NoiseCB info = {};
		info.noiseSize.x = noise->GetWidth();
		info.noiseSize.y = noise->GetHeight();

		ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Noise];
		cb->SetData(&info);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::HS);
		cb->Bind(eShaderStage::DS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}
}
