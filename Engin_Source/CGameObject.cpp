#include "CGameObject.h"

GameObject::GameObject()
	: mState(eState::Active)
{
	mComponents.resize((UINT)eComponentType::End);
}

GameObject::~GameObject()
{
}

void GameObject::Initalize()
{
	for (Component* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Initalize();
	}
}

void GameObject::Update()
{
	for (Component* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Update();
	}
}

void GameObject::FixedUpdate()
{
	for (Component* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->FixedUpdate();
	}
}

void GameObject::Render()
{
	for (Component* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Render();
	}
}

void GameObject::AddComponent(Component* comp)
{
	int order = comp->GetOrder();
	mComponents[order] = comp;
	mComponents[order]->SetOwner(this);
}
