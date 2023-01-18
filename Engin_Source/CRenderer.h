#pragma once
#include "CEngine.h"
#include "Math.h"
#include "CGraphicDevice_DX11.h"

#include "CMesh.h"

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

	// 버텍스 셰이더
	extern Microsoft::WRL::ComPtr <ID3DBlob> triangleVSBlob;
	extern Microsoft::WRL::ComPtr <ID3D11VertexShader> triangleVS;

	// 픽셀 셰이더
	extern Microsoft::WRL::ComPtr <ID3DBlob> trianglePSBlob;
	extern Microsoft::WRL::ComPtr <ID3D11PixelShader> trianglePS;

	// input Layout
	extern Microsoft::WRL::ComPtr <ID3D11InputLayout> triangleLayout;

	void Initialize();
	void Release();
}

