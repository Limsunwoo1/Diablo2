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

using namespace Math;
using namespace graphics;
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
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[];

	void Initialize();
	void Release();
}

