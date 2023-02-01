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
	Transform, // ��ġ ������ �����ϴ� ������Ʈ
	Camera,
	Mesh,
	Collider,
	UI,
	Script,
	End,
};