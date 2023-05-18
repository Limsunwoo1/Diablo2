#include "CEnums.h"

const char* charComponentType[(int)eComponentType::End]
{
	"None",
	"Transform",
	"Mesh",
	"Collider",
	"MeshRenderer",
	"SpriteRenderer",
	"Animator",
	"ParticleSystem",
	"Light",
	"UI",
	"Script",
};

const wchar_t* wcharComponentType[(int)eComponentType::End]
{
	L"None",
	L"Transform",
	L"Mesh",
	L"Collider",
	L"MeshRenderer",
	L"SpriteRenderer",
	L"Animator",
	L"ParticleSystem",
	L"Light",
	L"UI",
	L"Script",
};

const char* charResourceType[(int)eResourceType::End] =
{
	"Mesh",
	"Texture",
	"Material",
	"Sound",
	//"Font
	"Prefab",
	"MeshData",
	"GraphicsShader",
	"ComputeShader",
	"Script",
};

const wchar_t* wcharResourceType[(int)eResourceType::End] =
{
	L"Mesh",
	L"Texture",
	L"Material",
	L"Sound",
	//"Font
	L"Prefab",
	L"MeshData",
	L"GraphicsShader",
	L"ComputeShader",
	L"Script",
};