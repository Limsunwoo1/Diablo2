#pragma once
#include "CEngine.h"
#include "Math.h"
#include "CGraphicDevice_DX11.h"

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

	// ���ؽ� ����
	extern ID3D11Buffer* triangleBuffer;
	extern ID3DBlob* errorBlob;

	// �ε�������
	extern ID3D11Buffer* triangleIndexBuffer;
	// ��� ����
	extern ID3D11Buffer* triangleConstantBuffer;

	// ���ؽ� ���̴�
	extern ID3DBlob* triangleVSBlob;
	extern ID3D11VertexShader* triangleVS;

	// �ȼ� ���̴�
	extern ID3DBlob* trianglePSBlob;
	extern ID3D11PixelShader* trianglePS;

	// input Layout
	extern ID3D11InputLayout* triangleLayout;

	void Initialize();
	void Release();
}

