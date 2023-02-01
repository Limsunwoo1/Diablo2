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