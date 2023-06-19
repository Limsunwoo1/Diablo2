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
		gameObj->SetLayerType(type);

		return gameObj;
	}

	template <typename T>
	static T* Instantiate(eLayerType type, Scene* scene, bool late)
	{
		T* gameObj = new T();
		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObj);

		if (!late)
			gameObj->Initalize();
		else
			SceneManager::GetInstance()->AddLateInitObject(gameObj);

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
	static T* Instantiate(eLayerType type, GameObject* obj, bool nextParam)
	{
		SceneManager::GetInstance()->AddEvent(obj);
		obj->SetLayerType(type);

		return dynamic_cast<T*>(obj);
	}

	template <typename T>
	static T* Instantiate(eLayerType type, GameObject* obj, bool nextParam, eSceneType scene)
	{
		Scene* Scene = SceneManager::GetInstance()->GetScene(scene);
		Layer& layer = Scene->Scene::GetLayer(type);
		layer.AddGameObject(obj);

		obj->GameObject::SetLayerType(type);


		return dynamic_cast<T*>(obj);
	}

	template <typename T>
	static T* Instantiate(eLayerType type, eSceneType scenetype, T* obj)
	{
		Scene* scene = SceneManager::GetInstance()->GetScene(scenetype);
		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(obj);
		obj->Initalize();
		obj->SetLayerType(type);

		return obj;
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

		if(nextPram)
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

	static bool CompareWallObject(GameObject* a, GameObject* b)
	{
		Transform* aTr = a->GetComponent<Transform>();
		Transform* bTr = b->GetComponent<Transform>();

		Vector3 aPos = aTr->GetPosition() + aTr->GetOffset();
		Vector3 bPos = bTr->GetPosition() + bTr->GetOffset();

		return aPos.y >= bPos.y;
	}
}
