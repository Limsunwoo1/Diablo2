#pragma once
#include "Math.h"
#include <wrl.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name

#define CBSLOT_TRANSFORM		0
#define CBSLOT_MATERIAL			1
#define CBSLOT_GRID				2
#define CBSLOT_FADE				3
#define CBSLOT_ANIMATION		4
#define CBSLOT_NUMBEROFLIGHT	5

namespace graphics
{
	enum class eCBType
	{
		Transform,
		Material,
		Grid,
		Fade,
		Animation,
		Light,
		End,
	};

	enum class ValidationMode
	{
		Disabled,
		Enabled,
		GPU,
	};

	enum class eShaderStage
	{
		VS,
		HS,
		DS,
		GS,
		PS,
		CS,
		Count,
	};

	enum class eSamplerType
	{
		Point,
		Linear,
		Anisotropic,
		End,
	};

	enum class eRasterizeType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WirefameNone,
		End,
	};

	enum class eDepthStencilType
	{
		Less,
		Greater,
		NoWrite,
		None,
		End,
	};

	enum class eBlendType
	{
		Default,
		AlphaBlend,
		OneOne,
		End,
	};

	enum class eRenderingMode
	{
		Opaque,			 // 불투명
		CutOut,			 // 투명 + 불투명 물체
		Transparent,	 // 투명
		End,
	};

	enum class eGpuParam
	{
		Int,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Matrix,
	};

	enum class eSRVType
	{
		None,
		End,
	};

	struct GpuBuffer
	{
		enum class eType
		{
			Buffer,
			Texture,
			UnknownType,
		};

		D3D11_BUFFER_DESC desc = {};
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		eType type =  eType::UnknownType;

		GpuBuffer() = default;
		virtual ~GpuBuffer() = default;
	};

	struct DebugMesh
	{
		eColliderType type;
		Math::Vector3 position;
		Math::Vector3 rotation;
		Math::Vector3 scale;
		float radius;
		float duration;
		float time;
	};

	struct LightAttribute
	{
		Math::Vector4 diffuse;
		Math::Vector4 specular;
		Math::Vector4 ambient;
		Math::Vector4 position;
		Math::Vector4 direction;

		eLightType type;
		float radius;
		float angle;
		int padding;
	};
}