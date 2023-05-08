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