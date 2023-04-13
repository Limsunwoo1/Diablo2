#pragma once
#include "CSceneManager.h"
#include "CGameObject.h"


namespace Object
{
	template <typename T>
	static T* Instantiate(eLayerType type)
	{
		T* gameObject = new T();
		Scene* scene = SceneManager::GetInstance()->GetActiveScene();
		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObject);
		gameObject->Initalize();

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(eLayerType type, Scene* scene)
	{
		T* gameObj = new T();
		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObj);
		gameObj->Initalize();

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(eLayerType type, GameObject* obj)
	{
		Scene* scene = SceneManager::GetInstance()->GetActiveScene();
		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(obj);
		obj->Initalize();

		return dynamic_cast<T*>(obj);
	}

	template <typename T>
	static T* Instantiate(eLayerType type, eSceneType scenetype, GameObject* obj)
	{
		Scene* scene = SceneManager::GetInstance()->GetScene(scenetype);
		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(obj);
		obj->Initalize();
		obj->SetLayerType(type);

		return dynamic_cast<T*>(obj);
	}

	template <typename T>
	static T* Instantiate(eLayerType type, Transform* parent)
	{
		T* gameObject = new T();
		Scene* scene = SceneManager::GetInstance()->GetActiveScene();
		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObject);

		Transform* transform = gameObject->GetComponent<Transform>();
		transform->SetParent(parent);
		gameObject->Initalize();

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(eLayerType type, Vector3 position, Vector3 rotation)
	{
		T* gameObject = new T();
		Scene* scene = SceneManager::GetInstance()->GetActiveScene();
		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObject);

		Transform* transform = gameObject->GameObejct::GetComponent<Transform>();
		transform->SetPosition(position);
		transform->SetRotation(rotation);

		gameObject->Initalize();

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(eLayerType type, Vector3 position, Vector3 rotation, Transform* parent)
	{
		T* gameObject = new T();
		Scene* scene = SceneManager::GetInstance()->GetActiveScene();
		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObject);

		Transform* transform = gameObject->GameObejct::GetComponent<Transform>();
		transform->SetParent(parent);
		transform->SetPosition(position);
		transform->SetRotation(rotation);

		gameObject->Initalize();

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(eLayerType type, bool nextPram)
	{
		T* gameObject = new T();
		SceneManager::GetInstance()->AddEvent(gameObject);

		gameObject->GameObject::SetLayerType(type);
		gameObject->Initalize();

		return gameObject;
	}

	static void ObjectDestroy(GameObject* gameObject)
	{
		gameObject->Death();
	}

	static void DontDestroyOnLoad(GameObject* gameObject) // 씬 이동시 이 오브젝트는 삭제되지 않는다
	{
		if (gameObject == nullptr)
			return;

		gameObject->DontDestroy(true);
	}
}
