#pragma once
#include "CEnums.h"
#include "Math.h"

#define SINGLE(type)						\
public:										\
	static type* GetInstance()				\
	{										\
		static type* mInstance;				\
		if (mInstance == nullptr)			\
		{									\
			mInstance = new type();			\
			return mInstance;				\
		}									\
		return mInstance;					\
	}										\
											\
	static void DestroyInstance()			\
	{										\
	if (GetInstance())						\
		delete GetInstance();				\
	}										\
											\
private:									\
	type();									\
	~type();								\

#define GETSINGLE(type)	\
type::GetInstance()		\

// 반환형, 타겟, 생성되는 메소드명
#define GETSET(__TYPE__, __TARGET__, __METHODNAME__) \
__TYPE__ Get##__METHODNAME__()	const				 \
{													 \
	return __TARGET__;								 \
}													 \
													 \
void Set##__METHODNAME__(__TYPE__ value)			 \
{													 \
	__TARGET__ = value;								 \
}													 \


#define MAX_DATA_SIZE		1024
#define MAX_NAME_SIZE		20
#define MAX_PASSWORD_SIZE	40

#define NEW_SHARED(name) std::make_shared<name>();

#define FAIL_EVENT -1

#define ITEM_X_SIZE 54.f
#define ITEM_Y_SIZE 52.5f


// ImGui_internal.h
// ImGui.h 보다 먼저 선언 되어야 하므로 미리 선언 해둠
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif



#define TILE_X_HALF_SIZE 200
#define TILE_Y_HALF_SIZE 100

#define WORLD_OFFSET 5000




union Pos_Data
{
	struct
	{
		int posX;
		int posY;
	};

	UINT64 PosData;
};
union Size_Data
{
	struct
	{
		int sizeX;
		int sizeY;
	};

	UINT64 SizeData;
};
union Offset_Data
{
	struct
	{
		int offsetX;
		int offsetY;
	};

	UINT64 OffsetData;
};
union IDX_Data
{
	struct
	{
		int idxX;
		int idxY;
	};

	UINT64 ID;
};
union Screen_IDX_Data
{
	struct
	{
		int screenIdxX;
		int screenIdxY;
	};

	UINT64 ScrrenID;
};


struct TileCarveData
{
	int TileIdxX;
	int TileIdxY;

	int ArrValue;
	class TileObject* tile;
};


using namespace std;

struct Login_Packet
{
	ServerDataType type;

	string name;
	string password;
};

struct Logout_Packet
{
	ServerDataType type;

	string name;
};

struct ChatMassege_Packet
{
	ServerDataType type;

	string name;
	string Messege;
};

struct WhisperMessege_Packet
{
	ServerDataType type;

	string name;
	string Messege;

	string otherName;
};

struct Position_Packet
{
	ServerDataType type;

	Math::Vector3 position;
};

struct Animation_Packet
{
	ServerDataType type;

	int animationIDX;
	wstring animationName;
};