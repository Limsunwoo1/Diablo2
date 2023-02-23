#pragma once
#include "CEntity.h"
#include "CComponent.h"
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

	template <typename T>
	T* AddComponent()
	{
		T* component = new T();
		eComponentType order = component->GetOrder();

		if (order == eComponentType::Script)
		{
			mScripts.push_back(component);
			component->SetOwner(this);
		}
		else
		{
			mComponents[(UINT)order] = component;
			mComponents[(UINT)order]->SetOwner(this);
		}

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

	bool IsDead()
	{
		if (mState == eState::dead)
			return true;

		return false;
	}
	void Pused() { mState = eState::paused; }
	void Death() { mState = eState::dead; }
	void Active() { mState = eState::active; }
	eState GetState() { return mState; }


private:
	eState mState;
	std::vector<Component*> mComponents;
	std::vector<Component*> mScripts;
};

