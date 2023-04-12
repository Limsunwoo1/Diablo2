#pragma once

enum class eSceneType
{
	Title,
	Play,
	Selecte,
	Create,
	Endding,
	End,
};

enum class eLayerType
{
	None = 0,
	Camera,
	BackGround,
	Tile,
	Grid,
	MonsterSkil,
	PlayerSKil,
	Monster,
	Player,

	Particle,
	Effect,
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
	Animator,
	GenericAnimator,
	AStart,
	ParticleSystem,
	Light,
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

enum class eAnimationType
{
	None,
	SecondDimension,
	ThirdDimension,
	End,
};

enum class eLightType
{
	Directional,
	Point,
	Spot,
	End,
};

enum class eUIType
{
	Button,
	HUD,
	Panel,
	End
};

//enum class eObjectReturnType
//{
//	Skil,
//	Effect,
//	End,
//};