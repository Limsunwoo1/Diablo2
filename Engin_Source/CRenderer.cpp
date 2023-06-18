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
	Camera* IsometricCamera = nullptr;
	std::vector<Camera*> Cameras[(UINT)eSceneType::End];
	std::vector<DebugMesh> debugMeshes;
	std::vector<LightAttribute> lights;
	StructedBuffer* LightBuffer;

	std::shared_ptr<Texture2D> PostProcessTexture = nullptr;
	GameObject* InspectorGameObject = nullptr;

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
#pragma region Isometric MESH
		// Debug
		vertexes[0].pos = Vector4(0.0f, -0.5f, 0.0f, 1.0f);
		vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);
		vertexes[0].uv = Vector2(0.f, 0.f);

		vertexes[1].pos = Vector4(0.5f, 0.5f, 0.0f, 1.0f);
		vertexes[1].color = Vector4(1.f, 1.f, 1.f, 1.f);
		vertexes[1].uv = Vector2(1.f, 0.f);

		vertexes[2].pos = Vector4(0.0f, 0.5f, 0.0f, 1.0f);
		vertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
		vertexes[2].uv = Vector2(1.f, 1.f);

		vertexes[3].pos = Vector4(-0.5f, -0.5f, 0.0f, 1.0f);
		vertexes[3].color = Vector4(0.f, 0.f, 1.f, 1.f);
		vertexes[3].uv = Vector2(0.f, 1.f);

		std::shared_ptr<Mesh>IsoMetricMesh = std::make_shared<Mesh>();
		ResourceManager::GetInstance()->Insert<Mesh>(L"IsoMetricMesh", IsoMetricMesh);

		IsoMetricMesh->CreateVertexBuffer(vertexes, 4);
		IsoMetricMesh->CreateIndexBuffer(indexs.data(), (UINT)indexs.size());
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
		FadeInOut[0].color = Vector4(0.f, 0.f, 0.f, 1.0f);
		FadeInOut[0].uv = Vector2(0.f, 0.f);

		FadeInOut[1].pos = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
		FadeInOut[1].color = Vector4(0.f, 0.f, 0.f, 1.0f);
		FadeInOut[1].uv = Vector2(1.f, 0.f);

		FadeInOut[2].pos = Vector4(1.0f, -1.0f, 0.0f, 1.0f);
		FadeInOut[2].color = Vector4(0.f, 0.f, 0.f, 1.0f);
		FadeInOut[2].uv = Vector2(1.f, 1.f);

		FadeInOut[3].pos = Vector4(-1.0f, -1.0f, 0.0f, 1.0f);
		FadeInOut[3].color = Vector4(0.f, 0.f, 0.f, 1.0f);
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

		std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"RectShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader.lock()->GetVSBlobBufferPointer()
			, shader.lock()->GetVSBlobBufferSize()
			, shader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> spriteshader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, spriteshader.lock()->GetVSBlobBufferPointer()
			, spriteshader.lock()->GetVSBlobBufferSize()
			, spriteshader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> uishader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, uishader.lock()->GetVSBlobBufferPointer()
			, uishader.lock()->GetVSBlobBufferSize()
			, uishader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> gridshader = ResourceManager::GetInstance()->Find<Shader>(L"GridShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, gridshader.lock()->GetVSBlobBufferPointer()
			, gridshader.lock()->GetVSBlobBufferSize()
			, gridshader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> fadeShader = ResourceManager::GetInstance()->Find<Shader>(L"FadeShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, fadeShader.lock()->GetVSBlobBufferPointer()
			, fadeShader.lock()->GetVSBlobBufferSize()
			, fadeShader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> debugShader = ResourceManager::GetInstance()->Find<Shader>(L"DebugShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, debugShader.lock()->GetVSBlobBufferPointer()
			, debugShader.lock()->GetVSBlobBufferSize()
			, debugShader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> particleShader = ResourceManager::GetInstance()->Find<Shader>(L"ParticleShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, particleShader.lock()->GetVSBlobBufferPointer()
			, particleShader.lock()->GetVSBlobBufferSize()
			, particleShader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> pingShader = ResourceManager::GetInstance()->Find<Shader>(L"PingShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, pingShader.lock()->GetVSBlobBufferPointer()
			, pingShader.lock()->GetVSBlobBufferSize()
			, pingShader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> RungaugeShader = ResourceManager::GetInstance()->Find<Shader>(L"GaugeShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, RungaugeShader.lock()->GetVSBlobBufferPointer()
			, RungaugeShader.lock()->GetVSBlobBufferSize()
			, RungaugeShader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> ItemShader = ResourceManager::GetInstance()->Find<Shader>(L"ItemShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, ItemShader.lock()->GetVSBlobBufferPointer()
			, ItemShader.lock()->GetVSBlobBufferSize()
			, ItemShader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> ItemSlotShader = ResourceManager::GetInstance()->Find<Shader>(L"ItemSlotShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, ItemSlotShader.lock()->GetVSBlobBufferPointer()
			, ItemSlotShader.lock()->GetVSBlobBufferSize()
			, ItemSlotShader.lock()->GetInputLayoutAddressOf());

		std::weak_ptr<Shader> postProecssShader = ResourceManager::GetInstance()->Find<Shader>(L"PostProcessShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, postProecssShader.lock()->GetVSBlobBufferPointer()
			, postProecssShader.lock()->GetVSBlobBufferSize()
			, postProecssShader.lock()->GetInputLayoutAddressOf());

		postProecssShader.lock()->SetKey(L"PostProcessShader");

		std::weak_ptr<Shader> grid2Shader = ResourceManager::GetInstance()->Find<Shader>(L"Grid2Shader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, grid2Shader.lock()->GetVSBlobBufferPointer()
			, grid2Shader.lock()->GetVSBlobBufferSize()
			, grid2Shader.lock()->GetInputLayoutAddressOf());

		grid2Shader.lock()->SetKey(L"Grid2Shader");

		std::weak_ptr<Shader> TileShader = ResourceManager::GetInstance()->Find<Shader>(L"TileShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, TileShader.lock()->GetVSBlobBufferPointer()
			, TileShader.lock()->GetVSBlobBufferSize()
			, TileShader.lock()->GetInputLayoutAddressOf());

		TileShader.lock()->SetKey(L"TileShader");

		std::weak_ptr<Shader> ObjectShader = ResourceManager::GetInstance()->Find<Shader>(L"ObjectShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, ObjectShader.lock()->GetVSBlobBufferPointer()
			, ObjectShader.lock()->GetVSBlobBufferSize()
			, ObjectShader.lock()->GetInputLayoutAddressOf());

		ObjectShader.lock()->SetKey(L"ObjectShader");

		std::weak_ptr<Shader> CarveShader = ResourceManager::GetInstance()->Find<Shader>(L"CarveShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, CarveShader.lock()->GetVSBlobBufferPointer()
			, CarveShader.lock()->GetVSBlobBufferSize()
			, CarveShader.lock()->GetInputLayoutAddressOf());

		CarveShader.lock()->SetKey(L"CarveShader");

		std::weak_ptr<Shader> LightBolt = ResourceManager::GetInstance()->Find<Shader>(L"LightBoltShader");
		graphics::GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, LightBolt.lock()->GetVSBlobBufferPointer()
			, LightBolt.lock()->GetVSBlobBufferSize()
			, LightBolt.lock()->GetInputLayoutAddressOf());

		LightBolt.lock()->SetKey(L"LightBoltShader");

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

		constantBuffers[(UINT)eCBType::TileData] = new ConstantBuffer(eCBType::TileData);
		constantBuffers[(UINT)eCBType::TileData]->Create(sizeof(TileDataCB));

		constantBuffers[(UINT)eCBType::ObjectData] = new ConstantBuffer(eCBType::ObjectData);
		constantBuffers[(UINT)eCBType::ObjectData]->Create(sizeof(ObjectDataCB));

		constantBuffers[(UINT)eCBType::LightingData] = new ConstantBuffer(eCBType::LightingData);
		constantBuffers[(UINT)eCBType::LightingData]->Create(sizeof(LightingBoltCB));
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
		spriteShader->SetDepthStencil(eDepthStencilType::Less);
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
#pragma region GRID2 SHADER
		// Grid
		{
			std::shared_ptr<Shader> gridShader = std::make_shared<Shader>();
			gridShader->Create(eShaderStage::VS, L"Grid2VS.hlsl", "main");
			gridShader->Create(eShaderStage::PS, L"Grid2PS.hlsl", "main");
			gridShader->SetRasterize(eRasterizeType::SolidNone);
			gridShader->SetDepthStencil(eDepthStencilType::Less);
			gridShader->SetBlend(eBlendType::AlphaBlend);

			ResourceManager::GetInstance()->Insert<Shader>(L"Grid2Shader", gridShader);
		}
#pragma endregion
#pragma region FADE SHADER
		// FadeInOut
		std::shared_ptr<Shader> fadeShader = std::make_shared<Shader>();
		fadeShader->Create(eShaderStage::VS, L"FadeInOutVS.hlsl", "main");
		fadeShader->Create(eShaderStage::PS, L"FadeInOutPS.hlsl", "main");
		fadeShader->SetRasterize(eRasterizeType::SolidNone);
		fadeShader->SetDepthStencil(eDepthStencilType::Less);
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
#pragma region PostProcess Shader
		std::shared_ptr<Shader> postProcessshader = std::make_shared<Shader>();
		postProcessshader->Create(eShaderStage::VS, L"PostProcessVS.hlsl", "main");
		postProcessshader->Create(eShaderStage::PS, L"PostProcessPS.hlsl", "main");
		postProcessshader->SetDepthStencil(eDepthStencilType::NoWrite);

		ResourceManager::GetInstance()->Insert<Shader>(L"PostProcessShader", postProcessshader);
#pragma endregion
#pragma region Tile Shader
		std::shared_ptr<Shader> TileShader = std::make_shared<Shader>();
		TileShader->Create(eShaderStage::VS, L"TileVS.hlsl", "main");
		TileShader->Create(eShaderStage::PS, L"TilePS.hlsl", "main");
		TileShader->SetDepthStencil(eDepthStencilType::None);


		ResourceManager::GetInstance()->Insert<Shader>(L"TileShader", TileShader);
#pragma endregion
#pragma region Object Shader
		std::shared_ptr<Shader> ObjectShader = std::make_shared<Shader>();
		ObjectShader->Create(eShaderStage::VS, L"ObjectVS.hlsl", "main");
		ObjectShader->Create(eShaderStage::PS, L"ObjectPS.hlsl", "main");
		ObjectShader->SetDepthStencil(eDepthStencilType::None);


		ResourceManager::GetInstance()->Insert<Shader>(L"ObjectShader", ObjectShader);
#pragma endregion
#pragma region TileCarve Shader
		std::shared_ptr<Shader> CarveShader = std::make_shared<Shader>();
		CarveShader->Create(eShaderStage::VS, L"TileCarveVS.hlsl", "main");
		CarveShader->Create(eShaderStage::PS, L"TileCarvePS.hlsl", "main");
		CarveShader->SetDepthStencil(eDepthStencilType::None);


		ResourceManager::GetInstance()->Insert<Shader>(L"CarveShader", CarveShader);
#pragma endregion
#pragma region LightBolt Shader
		std::shared_ptr<Shader> LightBoltShader = std::make_shared<Shader>();
		LightBoltShader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		LightBoltShader->Create(eShaderStage::PS, L"LightBoltPS.hlsl", "main");
		LightBoltShader->SetDepthStencil(eDepthStencilType::None);


		ResourceManager::GetInstance()->Insert<Shader>(L"LightBoltShader", LightBoltShader);
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
		ResourceManager::GetInstance()->Load<Texture2D>(L"noise_03", L"noise//noise_03.jpg");
		ResourceManager::GetInstance()->Load<Texture2D>(L"Green", L"Green.png");
		//ResourceManager::GetInstance()->Load<Texture2D>(L"Diablo2_Town_Idle", L"diablo2_Town_Idle.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"CartoonSmoke", L"particle\\CartoonSmoke.png");

		ResourceManager::GetInstance()->Load<Texture2D>(L"PlayerWalk", L"Diablo2_Walk.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"PlayerRun", L"run.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"PlayerIdle", L"Diablo2_Idle.png");
		ResourceManager::GetInstance()->Load<Texture2D>(L"Shadow", L"Shadow.png");

		{
			std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"Tile", L"testTile.png");
			tex.lock()->SetMaxX(5);
			tex.lock()->SetMaxY(37);
		}
		{
			std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"Wall3Tile", L"Wall3.png");
			tex.lock()->SetMaxX(51);
			tex.lock()->SetMaxY(1);
		}
		{
			std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"WallTile", L"Wall.png");
			tex.lock()->SetMaxX(19);
			tex.lock()->SetMaxY(1);
		}

		{
			for (int i = 0; i < 54; ++i)
			{
				wstring name = L"";
				wstring path = L"";
				wstring count = to_wstring(i + 1);

				path = L"Object//Wall_" + count + L".png";
				name = L"Wall_" + count + L"Object";

				std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(name, path);
				tex.lock()->SetMaxX(1);
				tex.lock()->SetMaxY(1);
			}

			for (int i = 0; i < 17; ++i)
			{
				wstring name = L"";
				wstring path = L"";
				wstring count = to_wstring(900 + i + 1);

				path = L"Object//Wall_" + count + L".png";
				name = L"Wall_" + count + L"Object";

				std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(name, path);
				tex.lock()->SetMaxX(1);
				tex.lock()->SetMaxY(1);
			}
		}
#pragma endregion
#pragma region DYNAMIC
		// Create
		std::shared_ptr<Texture2D> uavTexture = std::make_shared<Texture2D>();
		uavTexture->Create(1024, 1024, DXGI_FORMAT_B8G8R8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
		ResourceManager::GetInstance()->Insert<Texture2D>(L"PaintTexture", uavTexture);
#pragma endregion
		// CS 과제용 노이즈텍스쳐
		ResourceManager::GetInstance()->Load<Texture2D>(L"Noise", L"noise.png");
		std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"Noise");
		texture.lock()->BindShaderResource(graphics::eShaderStage::CS, 2);
		texture.lock()->BindShaderResource(graphics::eShaderStage::PS, 2);

		PostProcessTexture = std::make_shared<Texture2D>();
		PostProcessTexture->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		PostProcessTexture->BindShaderResource(eShaderStage::PS, 60);
	}

	void LoadMaterial()
	{
#pragma region DEFAULT MATERIAL
		// Dafault
		std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"SmileTexture");
		std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"RectShader");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		ResourceManager::GetInstance()->Insert<Material>(L"RectMaterial", material);
#pragma endregion
#pragma region DEFAULT2 MATERIAL
		{
			// Dafault
			std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"SmileTexture");
			std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"RectShader");
			std::shared_ptr<Material> material = std::make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(eTextureSlot::T0, texture);
			material->SetRenderingMode(eRenderingMode::Transparent);
			ResourceManager::GetInstance()->Insert<Material>(L"Rect2Material", material);
		}
#pragma endregion
#pragma region PAINT MATERIAL
		{
			std::weak_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"PaintTexture");
			std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"PaintShader");
			std::shared_ptr<Material> material = std::make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(eTextureSlot::T0, texture);
			ResourceManager::GetInstance()->Insert<Material>(L"noiseMaterial", material);
		}
#pragma endregion
#pragma region SPTRITE MATERIAL
		std::weak_ptr<Texture2D> spriteTexture = ResourceManager::GetInstance()->Find<Texture2D>(L"DefaultSprite");
		std::weak_ptr<Shader> spriteShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
		spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
		spriteMaterial->SetShader(spriteShader);
		spriteMaterial->SetTexture(eTextureSlot::T0, spriteTexture);
		ResourceManager::GetInstance()->Insert<Material>(L"SpriteMaterial", spriteMaterial);
#pragma endregion
#pragma region UI MATERIAL
		// UI
		std::weak_ptr<Texture2D> uiTexture = ResourceManager::GetInstance()->Find<Texture2D>(L"HPBarTexture");
		std::weak_ptr<Shader> uiShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> uiMaterial = std::make_shared<Material>();
		uiMaterial->SetRenderingMode(eRenderingMode::Transparent);
		uiMaterial->SetShader(uiShader);
		uiMaterial->SetTexture(eTextureSlot::T0, uiTexture);
		ResourceManager::GetInstance()->Insert<Material>(L"UIMaterial", uiMaterial);
#pragma endregion
#pragma region HP MATERIAL
		// UI
		std::weak_ptr<Shader> HPShader = ResourceManager::GetInstance()->Find<Shader>(L"GaugeShader");
		std::shared_ptr<Material> HPMaterial = std::make_shared<Material>();
		HPMaterial->SetRenderingMode(eRenderingMode::Transparent);
		HPMaterial->SetShader(HPShader);
		ResourceManager::GetInstance()->Insert<Material>(L"HPMaterial", HPMaterial);
#pragma endregion
#pragma region MP MATERIAL
		// UI
		std::weak_ptr<Shader> MPShader = ResourceManager::GetInstance()->Find<Shader>(L"GaugeShader");
		std::shared_ptr<Material> MPMaterial = std::make_shared<Material>();
		MPMaterial->SetRenderingMode(eRenderingMode::Transparent);
		MPMaterial->SetShader(MPShader);
		ResourceManager::GetInstance()->Insert<Material>(L"MPMaterial", MPMaterial);
#pragma endregion
#pragma region RunGauge MATERIAL
		// UI
		std::weak_ptr<Shader> RungaugeShader = ResourceManager::GetInstance()->Find<Shader>(L"GaugeShader");
		std::shared_ptr<Material> RunGaugeMaterial = std::make_shared<Material>();
		RunGaugeMaterial->SetRenderingMode(eRenderingMode::Transparent);
		RunGaugeMaterial->SetShader(RungaugeShader);
		ResourceManager::GetInstance()->Insert<Material>(L"RunGaugeMaterial", RunGaugeMaterial);
#pragma endregion
#pragma region HP Panel MATERIAL
		// UI
		std::weak_ptr<Shader> hpPanelShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> hpPanelMaterial = std::make_shared<Material>();
		hpPanelMaterial->SetRenderingMode(eRenderingMode::Transparent);
		hpPanelMaterial->SetShader(hpPanelShader);
		ResourceManager::GetInstance()->Insert<Material>(L"HpPanelMaterial", hpPanelMaterial);
#pragma endregion
#pragma region MP Panel MATERIAL
		// UI
		std::weak_ptr<Shader> mpShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> mpMaterial = std::make_shared<Material>();
		mpMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		mpMaterial ->SetShader(mpShader);
		ResourceManager::GetInstance()->Insert<Material>(L"MpPanelMaterial", mpMaterial);
#pragma endregion
#pragma region RunIcon MATERIAL
		// UI
		std::weak_ptr<Shader> RunIconShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> RunIconMaterial = std::make_shared<Material>();
		RunIconMaterial->SetRenderingMode(eRenderingMode::Transparent);
		RunIconMaterial->SetShader(RunIconShader);
		ResourceManager::GetInstance()->Insert<Material>(L"RunIconMaterial", RunIconMaterial);
#pragma endregion
#pragma region WalkIcon MATERIAL
		// UI
		std::weak_ptr<Shader> WalkIconShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> WalkIconMaterial = std::make_shared<Material>();
		WalkIconMaterial->SetRenderingMode(eRenderingMode::Transparent);
		WalkIconMaterial->SetShader(WalkIconShader);
		ResourceManager::GetInstance()->Insert<Material>(L"WalkIconMaterial", WalkIconMaterial);
#pragma endregion
#pragma region Main MATERIAL
		// UI
		std::weak_ptr<Shader> mainShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> mainuiMaterial = std::make_shared<Material>();
		mainuiMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		mainuiMaterial ->SetShader(mainShader);
		ResourceManager::GetInstance()->Insert<Material>(L"mainPanelMaterial", mainuiMaterial);
#pragma endregion
#pragma region Main INVENTORY MATERIAL
		// UI
		std::weak_ptr<Shader> InventoryShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> InventoryMaterial = std::make_shared<Material>();
		InventoryMaterial->SetRenderingMode(eRenderingMode::Transparent);
		InventoryMaterial->SetShader(InventoryShader);
		ResourceManager::GetInstance()->Insert<Material>(L"InventoryMaterial", InventoryMaterial);
#pragma endregion
#pragma region GRID MATERIAL
		// Grid
		std::weak_ptr<Shader> GridShader = ResourceManager::GetInstance()->Find<Shader>(L"GridShader");
		std::shared_ptr<Material> gridMaterial = std::make_shared<Material>();
		gridMaterial->SetShader(GridShader);
		ResourceManager::GetInstance()->Insert(L"GridMaterial", gridMaterial);
#pragma endregion
#pragma region GRID2 MATERIAL
		// Grid
		{
			std::weak_ptr<Shader> GridShader = ResourceManager::GetInstance()->Find<Shader>(L"Grid2Shader");
			std::shared_ptr<Material> gridMaterial = std::make_shared<Material>();
			gridMaterial->SetShader(GridShader);
			ResourceManager::GetInstance()->Insert(L"Grid2Material", gridMaterial);
		}
#pragma endregion
#pragma region FADE MATERIAL
		// FadeInOut
		std::weak_ptr<Shader> FadeShader = ResourceManager::GetInstance()->Find<Shader>(L"FadeShader");
		std::shared_ptr<Material> FadeMaterial = std::make_shared<Material>();
		FadeMaterial->SetShader(FadeShader);
		ResourceManager::GetInstance()->Insert(L"FadeMaterial", FadeMaterial);
#pragma endregion
#pragma region DEBUG MATERIAL
		// Debug
		std::weak_ptr<Shader> debugShader = ResourceManager::GetInstance()->Find<Shader>(L"DebugShader");
		std::shared_ptr<Material> debugMaterial = std::make_shared<Material>();
		debugMaterial->SetRenderingMode(eRenderingMode::Transparent);
		debugMaterial->SetShader(debugShader);
		ResourceManager::GetInstance()->Insert<Material>(L"DebugMaterial", debugMaterial);
#pragma endregion
#pragma region PARTICLE MATERIAL
		// Particle
		{
			std::weak_ptr<Shader> particleShader = ResourceManager::GetInstance()->Find<Shader>(L"ParticleShader");
			std::shared_ptr<Material> particleMaterial = std::make_shared<Material>();
			particleMaterial->SetRenderingMode(eRenderingMode::Transparent);
			particleMaterial->SetShader(particleShader);
			ResourceManager::GetInstance()->Insert<Material>(L"ParticleMaterial", particleMaterial);
		}
#pragma endregion
#pragma region PingMaterial
		std::weak_ptr<Shader> pingShdaer = ResourceManager::GetInstance()->Find<Shader>(L"PingShader");
		std::shared_ptr<Material> pingMaterial = std::make_shared<Material>();
		pingMaterial->SetRenderingMode(eRenderingMode::Transparent);
		pingMaterial->SetShader(pingShdaer);
		ResourceManager::GetInstance()->Insert<Material>(L"PingMaterial", pingMaterial);
#pragma endregion
#pragma region FrozenOrb
		std::weak_ptr<Shader> frozenShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> FrozenOrbMater = std::make_shared<Material>();
		FrozenOrbMater->SetRenderingMode(eRenderingMode::Transparent);
		FrozenOrbMater->SetShader(frozenShader);
		ResourceManager::GetInstance()->Insert<Material>(L"FrozenOrbMaterial", FrozenOrbMater);
#pragma endregion
#pragma region FrozenMisile
		std::weak_ptr<Shader> frozenMisileShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> FrozenMisile = std::make_shared<Material>();
		FrozenMisile->SetRenderingMode(eRenderingMode::Transparent);
		FrozenMisile->SetShader(frozenMisileShader);
		ResourceManager::GetInstance()->Insert<Material>(L"FrozenMisileMaterial", FrozenMisile);
#pragma endregion
#pragma region TelePortMaterail
		std::weak_ptr<Shader> TeleParteShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> TelePort = std::make_shared<Material>();
		TelePort->SetRenderingMode(eRenderingMode::Transparent);
		TelePort->SetShader(TeleParteShader);
		ResourceManager::GetInstance()->Insert<Material>(L"TelePortMaterial", TelePort);
#pragma endregion
#pragma region AStarRenderMaterial
		std::weak_ptr<Shader> AstarShader = ResourceManager::GetInstance()->Find<Shader>(L"FadeShader");
		std::weak_ptr<Texture2D> green = ResourceManager::GetInstance()->Find<Texture2D>(L"Green");
		std::shared_ptr<Material> AstarMater = std::make_shared<Material>();
		AstarMater->SetRenderingMode(eRenderingMode::Transparent);
		AstarMater->SetShader(AstarShader);
		AstarMater->SetTexture(eTextureSlot::T0 ,green);
		ResourceManager::GetInstance()->Insert<Material>(L"AstarMaterial", AstarMater);
#pragma endregion
#pragma region MeteorMaterial
		std::weak_ptr<Shader> MeteorShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> MeteorMaterial = std::make_shared<Material>();
		MeteorMaterial->SetRenderingMode(eRenderingMode::Transparent);
		MeteorMaterial->SetShader(MeteorShader);
		ResourceManager::GetInstance()->Insert<Material>(L"MeteorMaterial", MeteorMaterial);
#pragma endregion
#pragma region FalmeMaterial
		std::weak_ptr<Shader> flameShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> flameMaterial = std::make_shared<Material>();
		flameMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		flameMaterial ->SetShader(flameShader);
		ResourceManager::GetInstance()->Insert<Material>(L"FlameMaterial", flameMaterial);
#pragma endregion
#pragma region FirePinMaterial
		std::weak_ptr<Shader> PinShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> firtPinMaterial = std::make_shared<Material>();
		firtPinMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		firtPinMaterial ->SetShader(PinShader);
		ResourceManager::GetInstance()->Insert<Material>(L"FirePinMaterial", firtPinMaterial);
#pragma endregion
#pragma region MinoTauros
		std::weak_ptr<Shader> MinoShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> MinoMaterial = std::make_shared<Material>();
		MinoMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		MinoMaterial ->SetShader(MinoShader);
		ResourceManager::GetInstance()->Insert<Material>(L"MinoMaterial", MinoMaterial);
#pragma endregion
#pragma region ItemSlot
		std::weak_ptr<Shader> ItemSlotShader = ResourceManager::GetInstance()->Find<Shader>(L"ItemSlotShader");
		std::shared_ptr<Material> ItemSlotMaterial = std::make_shared<Material>();
		ItemSlotMaterial->SetRenderingMode(eRenderingMode::Transparent);
		ItemSlotMaterial->SetShader(ItemSlotShader);
		ResourceManager::GetInstance()->Insert<Material>(L"ItemSlotMaterial", ItemSlotMaterial);
#pragma endregion
#pragma region HpPotion
		std::weak_ptr<Shader> HpShader = ResourceManager::GetInstance()->Find<Shader>(L"ItemShader");
		std::shared_ptr<Material> HpPotionMaterial = std::make_shared<Material>();
		HpPotionMaterial ->SetRenderingMode(eRenderingMode::Transparent);
		HpPotionMaterial ->SetShader(HpShader);
		ResourceManager::GetInstance()->Insert<Material>(L"HPPotionMaterial", HpPotionMaterial);
#pragma endregion
#pragma region MpPotion
		std::weak_ptr<Shader> MpShader = ResourceManager::GetInstance()->Find<Shader>(L"ItemShader");
		std::shared_ptr<Material> MpPotionMaterial = std::make_shared<Material>();
		MpPotionMaterial->SetRenderingMode(eRenderingMode::Transparent);
		MpPotionMaterial->SetShader(MpShader);
		ResourceManager::GetInstance()->Insert<Material>(L"MPPotionMaterial", MpPotionMaterial);
#pragma endregion
#pragma region PostProcess Material
		std::weak_ptr<Shader> postProcessShader = ResourceManager::GetInstance()->Find<Shader>(L"PostProcessShader");
		std::shared_ptr<Material> postProcessMaterial = std::make_shared<Material>();
		postProcessMaterial->SetRenderingMode(eRenderingMode::PostProcess);
		postProcessMaterial->SetShader(postProcessShader);
		ResourceManager::GetInstance()->Insert<Material>(L"PostProcessMaterial", postProcessMaterial);
#pragma endregion
#pragma region ChaterSelect Material
		std::weak_ptr<Shader> ChaterSelectShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> ChaterSelectMaterial = std::make_shared<Material>();
		ChaterSelectMaterial->SetRenderingMode(eRenderingMode::Transparent);
		ChaterSelectMaterial->SetShader(ChaterSelectShader);
		ResourceManager::GetInstance()->Insert<Material>(L"ChaterSelectMaterial", ChaterSelectMaterial);
#pragma endregion
#pragma region Button Material
		{
			std::weak_ptr<Shader> ButtonShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
			std::shared_ptr<Material> ButtonMaterial = std::make_shared<Material>();
			ButtonMaterial->SetRenderingMode(eRenderingMode::Transparent);
			ButtonMaterial->SetShader(ButtonShader);
			ResourceManager::GetInstance()->Insert<Material>(L"Button1Material", ButtonMaterial);
		}
#pragma endregion
#pragma region Button2 Material
		{
			std::weak_ptr<Shader> ButtonShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
			std::shared_ptr<Material> ButtonMaterial = std::make_shared<Material>();
			ButtonMaterial->SetRenderingMode(eRenderingMode::Transparent);
			ButtonMaterial->SetShader(ButtonShader);
			ResourceManager::GetInstance()->Insert<Material>(L"Button2Material", ButtonMaterial);
		}
#pragma endregion
#pragma region Button3 Material
		{
			std::weak_ptr<Shader> ButtonShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
			std::shared_ptr<Material> ButtonMaterial = std::make_shared<Material>();
			ButtonMaterial->SetRenderingMode(eRenderingMode::Transparent);
			ButtonMaterial->SetShader(ButtonShader);
			ResourceManager::GetInstance()->Insert<Material>(L"Button3Material", ButtonMaterial);;
		}
#pragma endregion

#pragma region	Tile Material
		{
			std::weak_ptr<Shader> TileShader = ResourceManager::GetInstance()->Find<Shader>(L"TileShader");
			std::shared_ptr<Material> TileMaterial = std::make_shared<Material>();
			TileMaterial ->SetRenderingMode(eRenderingMode::Transparent);
			TileMaterial ->SetShader(TileShader);
			ResourceManager::GetInstance()->Insert<Material>(L"TileMaterial", TileMaterial);;
		}
#pragma endregion
#pragma region Wall Material
		{
			std::weak_ptr<Shader> WallShader = ResourceManager::GetInstance()->Find<Shader>(L"TileShader");
			std::shared_ptr<Material> WallMaterial = std::make_shared<Material>();
			WallMaterial->SetRenderingMode(eRenderingMode::Transparent);
			WallMaterial->SetShader(WallShader);
			ResourceManager::GetInstance()->Insert<Material>(L"WallMaterial", WallMaterial);;
		}
#pragma endregion
#pragma region Carve Material
		{
			std::weak_ptr<Shader> CarveShader = ResourceManager::GetInstance()->Find<Shader>(L"CarveShader");
			std::shared_ptr<Material> CarveMaterial = std::make_shared<Material>();
			CarveMaterial->SetRenderingMode(eRenderingMode::Transparent);
			CarveMaterial->SetShader(CarveShader);
			ResourceManager::GetInstance()->Insert<Material>(L"CarveMaterial", CarveMaterial);
		}
#pragma endregion

#pragma region FireBolt Material
		{
			std::weak_ptr<Shader> FireBolt = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
			std::shared_ptr<Material> FireBoltMaterial = std::make_shared<Material>();
			FireBoltMaterial->SetRenderingMode(eRenderingMode::Transparent);
			FireBoltMaterial->SetShader(FireBolt);
			ResourceManager::GetInstance()->Insert<Material>(L"FireBoltMaterial", FireBoltMaterial);
		}
#pragma endregion
#pragma region FrozenBolt Material
		{
			std::weak_ptr<Shader> FrozenBolt = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
			std::shared_ptr<Material> FrozenBoltMaterial = std::make_shared<Material>();
			FrozenBoltMaterial->SetRenderingMode(eRenderingMode::Transparent);
			FrozenBoltMaterial->SetShader(FrozenBolt);
			ResourceManager::GetInstance()->Insert<Material>(L"FrozenBoltMaterial", FrozenBoltMaterial);
		}
#pragma endregion
#pragma region LightnBolt Material
		{
			std::weak_ptr<Shader> LightnBolt = ResourceManager::GetInstance()->Find<Shader>(L"LightBoltShader");
			std::shared_ptr<Material> LightnBolttMaterial = std::make_shared<Material>();
			LightnBolttMaterial->SetRenderingMode(eRenderingMode::Transparent);
			LightnBolttMaterial->SetShader(LightnBolt);
			ResourceManager::GetInstance()->Insert<Material>(L"LightBoltMaterial", LightnBolttMaterial);
		}
#pragma endregion
#pragma region Shadow Material
		{
			std::weak_ptr<Shader>  ShadowShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
			std::shared_ptr<Material>ShadowMaterial = std::make_shared<Material>();
			ShadowMaterial->SetRenderingMode(eRenderingMode::Transparent);
			ShadowMaterial->SetShader(ShadowShader);
			ResourceManager::GetInstance()->Insert<Material>(L"ShadowMaterial", ShadowMaterial);
		}
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

	float noiseTime = 10.0f;
	float time = 0.0f;
	void BindNoiseTexture()
	{
		std::weak_ptr<Texture2D> noise = ResourceManager::GetInstance()->Find<Texture2D>(L"noise_03");
		noise.lock()->BindShaderResource(eShaderStage::VS, 16);
		noise.lock()->BindShaderResource(eShaderStage::HS, 16);
		noise.lock()->BindShaderResource(eShaderStage::DS, 16);
		noise.lock()->BindShaderResource(eShaderStage::GS, 16);
		noise.lock()->BindShaderResource(eShaderStage::PS, 16);
		noise.lock()->BindShaderResource(eShaderStage::CS, 16);
		noise.lock()->BindShaderResource(eShaderStage::VS, 16);

		NoiseCB info = {};
		info.noiseSize.x = noise.lock()->GetWidth();
		info.noiseSize.y = noise.lock()->GetHeight();
		noiseTime -= Time::GetInstance()->DeltaTime();
		time += Time::GetInstance()->DeltaTime();
		info.noiseTime = noiseTime;
		info.noiseElapseTime = time;

		ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Noise];
		cb->SetData(&info);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::HS);
		cb->Bind(eShaderStage::DS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}

	void CopyRenderTarget()
	{
		std::weak_ptr<Texture2D> renderTarget =
			ResourceManager::GetInstance()->Find<Texture2D>(L"RenderTarget");

		ID3D11ShaderResourceView* srv = nullptr;
		GetDevice()->BindShaderResource(eShaderStage::PS, 60, &srv);

		ID3D11Texture2D* dest = PostProcessTexture->GetTexture().Get();
		ID3D11Texture2D* source = renderTarget.lock()->GetTexture().Get();

		GetDevice()->CopyResource(dest, source);

		PostProcessTexture->BindShaderResource(eShaderStage::PS, 60);
	}
}
