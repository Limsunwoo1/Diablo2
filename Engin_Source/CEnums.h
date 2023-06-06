#pragma once

enum class eSceneType
{
	
	Title,
	Tool,
	MainTitle,
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
	TilePllet,
	Tile,
	Wall,
	Grid,
	MonsterSkil,
	PlayerSKil,
	Monster,
	Player,

	Particle,
	Effect,
	Fade,
	PostProcess,
	UI,
	End = 16,
};

enum class eComponentType
{
	None,
	Transform, // 위치 데이터 수정하는 컴포넌트
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
	Prefab, // 파일로 저장된 정보
	MeshData,
	GraphicShader,
	ComputeShader, // 병렬처리가 가능하게 해주는 쉐이더
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

enum class eEquipmentType : short
{
	Shoes,
	Glove,
	Belt,
	Wepon,
	Suit,
	Cap,
	Potion,
	HpPotion,
	MpPotion,
};

//enum class eObjectReturnType
//{
//	Skil,
//	Effect,
//	End,
//};

enum class eCharType
{
	Sorceress,
	Druid,
	End
};

enum class eFileType
{
	Char,
	Tile,
	Resource,
	Prepeb,
	End,
};

// Resource 폴더 WallDirection.pmg 참조
enum class eWallType
{
	Type1,
	Type2,
	Type3,
	Type4,
	Type5,
	Type6,
	Type7,
	Type8,
	Type9,
	End,
};

enum class eToolRenderMode
{
	ALL,
	TILE,
	OBJECT,
	Unmovable_Area,
	END,
};

extern const char* charComponentType[(int)eComponentType::End];
extern const wchar_t* wcharComponentType[(int)eComponentType::End];
extern const char* charResourceType[(int)eResourceType::End];
extern const wchar_t* wcharResourceType[(int)eResourceType::End];