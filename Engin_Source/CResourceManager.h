#pragma once
#include "CResource.h"

using namespace std;

class ResourceManager
{
	SINGLE(ResourceManager);

public:
	typedef map<wstring, shared_ptr<Resource>>::iterator ResourceIter;

	template <typename T>
	shared_ptr<T> Find(const wstring& key)
	{
		ResourceIter iter = mResources.find(key);

		// ���ҽ��� �̹� map �ȿ� �����Ұ��
		if (iter != mResources.end())
		{
			return dynamic_pointer_cast<T>(iter->second);
		}

		return nullptr;
	}

	template <typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path)
	{
		shared_ptr<T> resource = Find<T>(key);
		// �ش� Ű�� �̹� �ε��Ȱ� ������ ���ҽ��� ��ȯ
		if (resource)
			return resource;

		// ���ҽ��� ���°��
		resource = make_shared<T>();
		if (FAILED(resource->Load(path)))
		{
			MessageBox(nullptr, L"image Load Failed!!", L"Error", MB_OK);
			assert(false);
			return nullptr;
		}

		resource->SetKey(key);
		resource->SetPath(path);

		mResources.insert(make_pair(key, dynamic_pointer_cast<Resource>(resource)));

		return resource;
	}

	template <typename T>
	void Insert(const wstring& key, shared_ptr<T>resource)
	{
		if (key == L"")
		{
			MessageBox(nullptr, L"Image Load Failed", L"Input Key", MB_OK);
			return;
		}

		if(Find<T>(key) == nullptr)
			mResources.insert(make_pair(key, dynamic_pointer_cast<Resource>(resource)));
	}

	//void Release(void)
	//{
	//	ResourceIter iter = mResource.begin();
	//	for (; iter != mResource.end(); ++iter)
	//	{
	//		if (iter->second == nullptr)
	//			continue;

	//		delete (iter->second);
	//		iter->second = nullptr;
	//	}
	//}

private:
	map<wstring, shared_ptr<Resource>> mResources;
};


