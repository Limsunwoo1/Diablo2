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

	// ���� ������
	extern Vertex vertexes[4];

	// ����
	extern Mesh* mesh;
	// ��� ����
	extern Microsoft::WRL::ComPtr <ID3D11Buffer> triangleConstantBuffer;

	// ���ؽ� ���̴�
	extern Microsoft::WRL::ComPtr <ID3DBlob> triangleVSBlob;
	extern Microsoft::WRL::ComPtr <ID3D11VertexShader> triangleVS;

	// �ȼ� ���̴�
	extern Microsoft::WRL::ComPtr <ID3DBlob> trianglePSBlob;
	extern Microsoft::WRL::ComPtr <ID3D11PixelShader> trianglePS;

	// input Layout
	extern Microsoft::WRL::ComPtr <ID3D11InputLayout> triangleLayout;

	void Initialize();
	void Release();
}

