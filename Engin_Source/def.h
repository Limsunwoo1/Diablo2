#pragma once

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
