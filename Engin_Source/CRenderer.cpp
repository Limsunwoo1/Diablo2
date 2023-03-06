#include "CRenderer.h"
#include "CResourceManager.h"
#include "CMaterial.h"
#include "CSceneManager.h"

namespace Renderer
{
	Vertex vertexes[4] = {};
	Vertex FadeInOut[4] = {};
	ConstantBuffer* constantBuffers[(UINT)eCBType::End] = {};
	ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};
	ComPtr<ID3D11RasterizerState> RasterizeState[(UINT)eRasterizeType::End] = {};
	ComPtr<ID3D11DepthStencilState> Depth_StencilState[(UINT)eDepthStencilType::End] = {};
	ComPtr<ID3D11BlendState> BlendState[(UINT)eBlendType::End] = {};

	Camera* mainCamera = nullptr;
	std::vector<Camera*> Cameras[(UINT)eSceneType::End];
	std::vector<DebugMesh> debugMeshes;

	void LoadMesh()
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

		// Fade
		FadeInOut[0].pos = Vector4(-1.0f, 1.0f, 0.5f, 1.0f);
		FadeInOut[0].color = Vector4(0.f, 0.f, 0.f, 0.f);
		FadeInOut[0].uv = Vector2(0.f, 0.f);

		FadeInOut[1].pos = Vector4(1.0f, 1.0f, 0.5f, 1.0f);
		FadeInOut[1].color = Vector4(0.f, 0.f, 0.f, 1.f);
		FadeInOut[1].uv = Vector2(1.f, 0.f);

		FadeInOut[2].pos = Vector4(1.0f, -1.0f, 0.5f, 1.0f);
		FadeInOut[2].color = Vector4(0.f, 0.f, 0.f, 1.f);
		FadeInOut[2].uv = Vector2(1.f, 1.f);

		FadeInOut[3].pos = Vector4(-1.0f, -1.0f, 0.5f, 1.0f);
		FadeInOut[3].color = Vector4(0.f, 0.f, 0.f, 1.f);
		FadeInOut[3].uv = Vector2(0.f, 1.f);

		// Create Mesh
		std::shared_ptr<Mesh>mesh = std::make_shared<Mesh>();
		ResourceManager::GetInstance()->Insert<Mesh>(L"RectMesh", mesh);

		mesh->CreateVertexBuffer(vertexes, 4);

		std::shared_ptr<Mesh>Fademesh = std::make_shared<Mesh>();
		ResourceManager::GetInstance()->Insert<Mesh>(L"FadeMesh", Fademesh);

		Fademesh->CreateVertexBuffer(FadeInOut, 4);

		std::vector<UINT> indexs;
		indexs.push_back(0);
		indexs.push_back(1);
		indexs.push_back(2);

		indexs.push_back(0);
		indexs.push_back(2);
		indexs.push_back(3);
		indexs.push_back(0);

		mesh->CreateIndexBuffer(indexs.data(), (UINT)indexs.size());
		Fademesh->CreateIndexBuffer(indexs.data(), (UINT)indexs.size());

		// Circle Mesh
		std::vector<Vertex> circleVertex;
		Vertex center = {};
		center.pos = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		center.color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		center.uv = Vector2::Zero;

		circleVertex.push_back(center);

		int iSlice = 40;
		float fRadius = 0.5f;
		float fTheta = XM_2PI / (float)iSlice;

		for (int i = 0; i < iSlice; ++i)
		{
			Vertex vtx = {};
			vtx.pos = Vector4
			(
				fRadius * cosf(fTheta * (float)i)
				, fRadius * sinf(fTheta * (float)i)
				, 0.5f, 1.0f
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
	}


	void SetUpState()
	{
#pragma region InputLayout
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
		constantBuffers[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		constantBuffers[(UINT)eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffers[(UINT)eCBType::Material]->Create(sizeof(MaterialCB));

		constantBuffers[(UINT)eCBType::Grid] = new ConstantBuffer(eCBType::Grid);
		constantBuffers[(UINT)eCBType::Grid]->Create(sizeof(GridCB));

		constantBuffers[(UINT)eCBType::Fade] = new ConstantBuffer(eCBType::Fade);
		constantBuffers[(UINT)eCBType::Fade]->Create(sizeof(FadeCB));
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

		// UI
		std::shared_ptr<Shader> uiShader = std::make_shared<Shader>();
		uiShader->Create(eShaderStage::VS, L"UserInterfaceVS.hlsl", "main");
		uiShader->Create(eShaderStage::PS, L"UserInterfacePS.hlsl", "main");

		ResourceManager::GetInstance()->Insert<Shader>(L"UIShader", uiShader);

		// Grid
		std::shared_ptr<Shader> gridShader = std::make_shared<Shader>();
		gridShader->Create(eShaderStage::VS, L"GridVS.hlsl", "main");
		gridShader->Create(eShaderStage::PS, L"GridPS.hlsl", "main");
		gridShader->SetRasterize(eRasterizeType::SolidNone);
		gridShader->SetDepthStencil(eDepthStencilType::NoWrite);
		gridShader->SetBlend(eBlendType::AlphaBlend);


		ResourceManager::GetInstance()->Insert<Shader>(L"GridShader", gridShader);

		// FadeInOut
		std::shared_ptr<Shader> fadeShader = std::make_shared<Shader>();
		fadeShader->Create(eShaderStage::VS, L"FadeInOutVS.hlsl", "main");
		fadeShader->Create(eShaderStage::PS, L"FadeInOutPS.hlsl", "main");
		fadeShader->SetRasterize(eRasterizeType::SolidNone);
		fadeShader->SetDepthStencil(eDepthStencilType::NoWrite);
		fadeShader->SetBlend(eBlendType::AlphaBlend);

		ResourceManager::GetInstance()->Insert<Shader>(L"FadeShader", fadeShader);

		// Debug Shader
		std::shared_ptr<Shader> DebugShader = std::make_shared<Shader>();
		DebugShader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
		DebugShader->Create(eShaderStage::PS, L"DebugPS.hlsl", "main");

		DebugShader->SetRasterize(eRasterizeType::SolidNone);
		DebugShader->SetDepthStencil(eDepthStencilType::NoWrite);
		DebugShader->SetBlend(eBlendType::AlphaBlend);

		DebugShader->SetToplogy(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		ResourceManager::GetInstance()->Insert<Shader>(L"DebugShader", DebugShader);
	}

	void LoadTexture()
	{
		ResourceManager::GetInstance()->Load<Texture>(L"SmileTexture", L"Smile.png");
		ResourceManager::GetInstance()->Load<Texture>(L"DefaultSprite", L"Light.png");
		ResourceManager::GetInstance()->Load<Texture>(L"HPBarTexture", L"HPBar.png");

		ResourceManager::GetInstance()->Load<Texture>(L"Diablo2_Town_Idle", L"diablo2_Town_Idle.png");
	}

	void LoadMaterial()
	{
		// Dafault
		std::shared_ptr<Texture> texture = ResourceManager::GetInstance()->Find<Texture>(L"SmileTexture");
		std::shared_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"RectShader");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(texture);
		ResourceManager::GetInstance()->Insert<Material>(L"RectMaterial", material);


		// Sprite
		std::shared_ptr <Texture> spriteTexture = ResourceManager::GetInstance()->Find<Texture>(L"DefaultSprite");
		std::shared_ptr<Shader> spriteShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
		spriteMaterial->SetRenderingMode(eRenderingMode::Transparent);
		spriteMaterial->SetShader(spriteShader);
		spriteMaterial->SetTexture(spriteTexture);
		ResourceManager::GetInstance()->Insert<Material>(L"SpriteMaterial", spriteMaterial);

		// Diablo_Walk
		std::shared_ptr <Texture> DiabloTexture = ResourceManager::GetInstance()->Find<Texture>(L"Diablo2_Town_Idle");
		std::shared_ptr<Shader> DiabloShader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");
		std::shared_ptr<Material> DiabloMaterial = std::make_shared<Material>();
		DiabloMaterial->SetRenderingMode(eRenderingMode::Transparent);
		DiabloMaterial->SetShader(DiabloShader);
		DiabloMaterial->SetTexture(DiabloTexture);
		ResourceManager::GetInstance()->Insert<Material>(L"DiabloMaterial", DiabloMaterial);

		// UI
		std::shared_ptr <Texture> uiTexture = ResourceManager::GetInstance()->Find<Texture>(L"HPBarTexture");
		std::shared_ptr<Shader> uiShader = ResourceManager::GetInstance()->Find<Shader>(L"UIShader");
		std::shared_ptr<Material> uiMaterial = std::make_shared<Material>();
		uiMaterial->SetRenderingMode(eRenderingMode::Transparent);
		uiMaterial->SetShader(uiShader);
		uiMaterial->SetTexture(uiTexture);
		ResourceManager::GetInstance()->Insert<Material>(L"UIMaterial", uiMaterial);

		// Grid
		std::shared_ptr<Shader> GridShader = ResourceManager::GetInstance()->Find<Shader>(L"GridShader");
		std::shared_ptr<Material> gridMaterial = std::make_shared<Material>();
		gridMaterial->SetShader(GridShader);
		ResourceManager::GetInstance()->Insert(L"GridMaterial", gridMaterial);

		// FadeInOut
		std::shared_ptr<Shader> FadeShader = ResourceManager::GetInstance()->Find<Shader>(L"FadeShader");
		std::shared_ptr<Material> FadeMaterial = std::make_shared<Material>();
		FadeMaterial->SetShader(FadeShader);
		ResourceManager::GetInstance()->Insert(L"FadeMaterial", FadeMaterial);

		// Debug
		std::shared_ptr<Shader> debugShader = ResourceManager::GetInstance()->Find<Shader>(L"DebugShader");
		std::shared_ptr<Material> debugMaterial = std::make_shared<Material>();
		debugMaterial->SetRenderingMode(eRenderingMode::Transparent);
		debugMaterial->SetShader(debugShader);
		ResourceManager::GetInstance()->Insert<Material>(L"DebugMaterial", debugMaterial);
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
		eSceneType type = SceneManager::GetInstance()->GetActiveScene()->GetScenType();
		for (Camera* cam : Cameras[(UINT)type])
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		Cameras[(UINT)type].clear();
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
