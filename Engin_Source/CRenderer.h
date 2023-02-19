#pragma once
#include "CEngine.h"
#include "Math.h"
#include "CGraphicDevice_DX11.h"

#include "CMesh.h"
#include "CommonInclude.h"
#include "CGraphicDevice_DX11.h"

#include "CShader.h"
#include "CMesh.h"
#include "CConstantBuffer.h"
#include "CCamera.h"

using namespace Math;
using namespace graphics;
using namespace Microsoft::WRL;
namespace Renderer
{
	struct Vertex
	{
		Vector4 pos;
		Vector4 color;
		Vector2 uv;
	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	CBUFFER(MaterialCB, CBSLOT_MATERIAL)
	{
		int iData;
		float fData;
		Vector2 xy;
		Vector3 xyz;
		Vector4 xyzw;
		Matrix matrix;
	};

	extern Vertex vertexes[4];
	extern ConstantBuffer* constantBuffers[];
	extern ComPtr<ID3D11SamplerState> samplerStates[];
	extern ComPtr<ID3D11RasterizerState> RasterizeState[];
	extern ComPtr<ID3D11DepthStencilState> Depth_StencilState[];
	extern ComPtr<ID3D11BlendState> BlendState[];

	extern std::vector<Camera*> Cameras;

	void Initialize();
	void Release();
	void Render();
}

