#pragma once
#include "CEngine.h"
#include "Math.h"
#include "CGraphicDevice_DX11.h"

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

	CBUFFER(GridCB, CBSLOT_GRID)
	{
		Vector4 cameraPosition;
		Vector2 cameraScale;
		Vector2 resolution;
	};

	CBUFFER(FadeCB, CBSLOT_FADE)
	{
		float alpha;
	};

	extern Vertex FadeInOut[4];
	extern Vertex vertexes[4];
	extern ConstantBuffer* constantBuffers[];
	extern ComPtr<ID3D11SamplerState> samplerStates[];
	extern ComPtr<ID3D11RasterizerState> RasterizeState[];
	extern ComPtr<ID3D11DepthStencilState> Depth_StencilState[];
	extern ComPtr<ID3D11BlendState> BlendState[];

	extern std::vector<Camera*> Cameras[(UINT)eSceneType::End];

	void Initialize();
	void Release();
	void Render();
}

