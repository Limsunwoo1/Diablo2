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
#include "CStructedBuffer.h"

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
		float padding1;
		float padding2;
		float padding3;
	};

	CBUFFER(AnimationCB, CBSLOT_ANIMATION)
	{
		Vector2 leftTop;
		Vector2 size;
		Vector2 offset;
		Vector2 atlasSize;

		UINT type;
		UINT elementType;
	};

	// NumberOfLight
	CBUFFER(LightCB, CBSLOT_NUMBEROFLIGHT)
	{
		UINT numberOfLight;
	};

	CBUFFER(TimeCB, CBSLOT_TIME)
	{
		float deltatime;
		float time;
		float padding_;
		float padding_1;
	};

	CBUFFER(ParticleSystemCB, CBSLOT_PARTICLESYSTEM)
	{
		Vector4 worldPosition;
		Vector4 startColor;
		Vector4 startSize;

		UINT maxParticles;
		UINT simulationSpace;
		float radius;
		float startSpeed;

		float startLifeTime;
		float deltaTime;
		float elapsedTime; // 누적시간
		int padding;
	};

	CBUFFER(NoiseCB, CBSLOT_NOISE)
	{
		Vector4 noiseSize;
	};

	CBUFFER(PlayerDataCB, CBSLOT_PLAYERDATA)
	{
		float hpGauge;
		float mpGauge;
		float RunGauge;
		float expGauge;
	};

	CBUFFER(ItemDataCB, CBSLOT_ITEMDATA)
	{
		Vector4 canversColor;

		UINT stage;
		UINT drop;
		UINT pick;

	};
	extern Vertex FadeInOut[4];
	extern Vertex vertexes[4];
	extern Vertex HalfAlpha[4];
	extern ConstantBuffer* constantBuffers[];
	extern ComPtr<ID3D11SamplerState> samplerStates[];
	extern ComPtr<ID3D11RasterizerState> RasterizeState[];
	extern ComPtr<ID3D11DepthStencilState> Depth_StencilState[];
	extern ComPtr<ID3D11BlendState> BlendState[];

	extern std::vector<Camera*> Cameras[];
	extern std::vector<DebugMesh> debugMeshes;
	extern std::vector<LightAttribute> lights;
	extern StructedBuffer* LightBuffer;

	extern Camera* mainCamera;
	extern Camera* UiCamera;

	extern float Time;

	void Initialize();
	void Render();
	void Release();

	// Renderer
	void PushLightAttribute(LightAttribute lightAttribute);
	void BindLights();
	void BindTime();
	void BindNoiseTexture();
}

