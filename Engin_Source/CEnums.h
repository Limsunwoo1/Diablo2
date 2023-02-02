#pragma once

enum class eCBType
{
	Transform,
	End,
};

enum class eLayerType
{
	None = 0,
	Monster,
	Player,
	End = 16,
};

enum class eComponentType
{
	None,
	Transform, // 위치 데이터 수정하는 컴포넌트
	Camera,
	Mesh,
	Collider,
	UI,
	Script,
	End,
};

enum class eResourceType
{
	Mesh,
	Texture,
	Meterial,
	Sound,
	/*Font,*/
	Prefab, // 파일로 저장된 정보
	MeshData,
	GraphicShader,
	ComputeShader, // 병렬처리가 가능하게 해주는 쉐이더
	Script,
	End,
};