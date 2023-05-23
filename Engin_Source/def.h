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



#define TILE_X_SIZE 32
#define TILE_Y_SIZE 16