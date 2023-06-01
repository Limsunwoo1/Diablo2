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
// ImGui.h ���� ���� ���� �Ǿ�� �ϹǷ� �̸� ���� �ص�
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif



#define TILE_X_HALF_SIZE 200
#define TILE_Y_HALF_SIZE 100

#define WORLD_OFFSET 5000






union Tile_IDX_Data
{
	struct
	{
		int idxX;
		int indY;
	};

	UINT64 ID;
};