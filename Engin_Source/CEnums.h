#pragma once

enum class eSceneType
{
	Title,
	Play,
	End,
};

enum class eLayerType
{
	None = 0,
	Camera,
	Grid,
	Monster,
	Player,

	Fade,
	UI,
	End = 16,
};

enum class eComponentType
{
	None,
	Transform, // ��ġ ������ �����ϴ� ������Ʈ
	Camera,
	Mesh,
	Collider,
	MeshRenderer,
	SpriteRenderer,
	FadeRenderer,
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
	Prefab, // ���Ϸ� ����� ����
	MeshData,
	GraphicShader,
	ComputeShader, // ����ó���� �����ϰ� ���ִ� ���̴�
	Script,
	End,
};

enum class eColliderType
{
	None,
	Rect,
	Circle,
	Box,
	Sphere,
	End,
};
