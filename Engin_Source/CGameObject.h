#pragma once
#include "CEntity.h"
#include "CTransform.h"
#include "CScript.h"

class GameObject : public Entity
{
public:
	enum eState
	{
		active,
		paused,
		dead,
	};

	GameObject();
	virtual ~GameObject();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	virtual void InitAnimation();

	template <typename T>
	T* AddComponent()
	{
		T* component = new T();
		eComponentType order = component->GetOrder();

		if (order == eComponentType::Script)
		{
			mScripts.push_back(dynamic_cast<Script*>(component));
			component->SetOwner(this);
		}
		else
		{
			mComponents[(UINT)order] = component;
			mComponents[(UINT)order]->SetOwner(this);
		}
		component->Initalize();

		return component;
	}
	void AddComponent(Component* comp);

	template <typename T>
	T* GetComponent()
	{
		T* comp;
		for (auto c : mComponents)
		{
			comp = dynamic_cast<T*>(c);

			if (comp != nullptr)
				return comp;
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> components = {};
		T* comp;
		for (auto c : mComponents)
		{
			comp = dynamic_cast<T*>(c);

			if (comp != nullptr)
				components.push_back(comp);
		}

		return components;
	}

	template <typename T>
	T* GetScript()
	{
		T* comp;
		for (auto c : mScripts)
		{
			comp = dynamic_cast<T*>(c);

			if (comp != nullptr)
				return comp;
		}

		return nullptr;
	}

	const std::vector<Script*>& GetScripts() { return mScripts; }

	bool IsDead()
	{
		if (mState == eState::dead)
			return true;

		return false;
	}
	void Paused() { mState = eState::paused; }
	void Death() { mState = eState::dead; }
	void Active() { mState = eState::active; }
	eState GetState() { return mState; }

	bool IsDontDestroty() { return mbDontDestroy; }
	void DontDestroy(bool enable) { mbDontDestroy = enable; }

	eLayerType GetLayerType() { return mType; }
	void SetLayerType(eLayerType type) { mType = type; }
protected:
	std::vector<Component*> mComponents;

private:
	eState mState;
	eLayerType mType;
	std::vector<Script*> mScripts;
	bool mbDontDestroy;
};

