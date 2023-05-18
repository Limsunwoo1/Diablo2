#pragma once
#include "CEngine.h"
#include "CEntity.h"

	class Resource : public Entity
	{
	public:
		Resource(eResourceType type);
		virtual ~Resource();

		virtual HRESULT Load(const std::wstring& path) = 0;

		const std::wstring& GetKey() { return mKey; }
		const std::wstring& GetPath() { return mPath; }
		
		const void SetKey(const std::wstring key) { mKey = key; }
		const void SetPath(const std::wstring path) { mPath = path; }

		eResourceType GetType() { return mType; }
	private:
		std::wstring mPath;
		std::wstring mKey;

		eResourceType mType;
	};


