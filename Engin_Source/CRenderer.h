#pragma once
#include "CEngine.h"
#include "Math.h"
#include "CGraphicDevice_DX11.h"

#include "CMesh.h"
#include "CShader.h"

using namespace Math;
using namespace graphics;
namespace Renderer
{
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};

	// 정점 데이터
	extern Vertex vertexes[4];

	// 버퍼
	extern Mesh* mesh;
	// 상수 버퍼
	extern Microsoft::WRL::ComPtr <ID3D11Buffer> triangleConstantBuffer;
	// 쉐이더
	extern Shader* shader;

	void Initialize();
	void Release();
}

