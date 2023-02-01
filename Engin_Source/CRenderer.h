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
		Vector3 pos;
		Vector4 color;
	};

	extern Vertex vertexes[4];
	extern Mesh* mesh;
	extern Shader* shader;
	extern ConstantBuffer* constantBuffers[];

	void Initialize();
	void Release();
}

