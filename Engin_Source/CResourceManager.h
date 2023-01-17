#pragma once
#include "CResource.h"


	class ResourceManager
	{
		SINGLE(ResourceManager);

	public:
		typedef std::map<std::wstring, Resource*>::iterator ResourceIter;

		template <typename T>
		T* Find(const std::wstring& key)
		{
			ResourceIter iter = mResource.find(key);

			// ���ҽ��� �̹� map �ȿ� �����Ұ��
			if (iter != mResource.end())
			{
				return dynamic_cast<T*>(iter->second);
			}

			return nullptr;
		}

		template <typename T>
		T* Load(const std::wstring& key, const std::wstring& path)
		{
			T* resource = Find<T>(key);
			// �ش� Ű�� �̹� �ε��Ȱ� ������ ���ҽ��� ��ȯ
			if (resource)
				return resource;

			// ���ҽ��� ���°��
			resource = new T();
			if (FAILED(resource->Load(path)))
			{
				MessageBox(nullptr, L"image Load Failed!!", L"Error", MB_OK);
				delete resource;
				return nullptr;
			}

			resource->SetKey(key);
			resource->SetPath(path);

			mResource.insert(std::make_pair(key, resource));

			return dynamic_cast<T*>(resource);
		}

		template <typename T>
		void Insert(const std::wstring& key, T* resource)
		{
			if (resource == nullptr
				|| key == L"")
				return;

			mResource.insert(std::make_pair(key, resource));
		}

		void Release(void)
		{
			ResourceIter iter = mResource.begin();
			for (; iter != mResource.end(); ++iter)
			{
				if (iter->second == nullptr)
					continue;

				delete (iter->second);
				iter->second = nullptr;
			}
		}

	private:
		std::map<std::wstring, Resource*> mResource;
	};


