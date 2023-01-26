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

	// ���� ������
	extern Vertex vertexes[4];

	// ����
	extern Mesh* mesh;
	// ��� ����
	extern Microsoft::WRL::ComPtr <ID3D11Buffer> triangleConstantBuffer;
	// ���̴�
	extern Shader* shader;

	void Initialize();
	void Release();
}

