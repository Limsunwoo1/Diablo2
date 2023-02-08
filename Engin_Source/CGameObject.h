#pragma once
#include "CComponent.h"
#include "CScript.h"

class GameObject
{
public:
	enum eState
	{
		Active,
		Paused,
		Dead,
	};

	GameObject();
	virtual ~GameObject();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

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

private:
	eState mState;
	std::vector<Component*> mComponents;
	std::vector<Component*> mScripts;
};

