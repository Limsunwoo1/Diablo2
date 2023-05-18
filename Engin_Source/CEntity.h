#pragma once
#include "CEngine.h"
#include "Math.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();


	void SetName(const std::wstring name) { mName = name; }
	std::wstring GetName() { return mName; }
	UINT32 GetID() { return mID; }

private:
	std::wstring mName;
	UINT32 mID;
};

namespace gui
{
	class guiEntity
	{
	public:
		guiEntity();
		guiEntity(const char* name);
		virtual ~guiEntity();


		void SetName(const std::string name) { mName = name; }
		std::string GetName() { return mName; }
		UINT32 GetID() { return mID; }

	private:
		std::string mName;
		UINT32 mID;
	};
}


