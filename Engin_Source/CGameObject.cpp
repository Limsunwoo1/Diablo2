#include "CGameObject.h"

GameObject::GameObject()
	: mState(eState::Active)
{
	mComponents.resize((UINT)eComponentType::End);
}

GameObject::~GameObject()
{
	for (Component* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		delete comp;
		comp = nullptr;
	}

	for (Component* scrComp : mScripts)
	{
		if (scrComp == nullptr)
			continue;

		delete scrComp;
		scrComp = nullptr;
	}
}

void GameObject::Initalize()
{
	for (Component* comp : mComponents)
	{
		if (comp == nullptr)
			continue;

		comp->Initalize();
	}

	for (Component* script : mScripts)
	{
		if (script == nullptr)
			continue;

		script->Initalize();
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

	for (Component* script : mScripts)
	{
		if (script == nullptr)
			continue;

		script->Update();
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

	for (Component* script : mScripts)
	{
		if (script == nullptr)
			continue;

		script->FixedUpdate();
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

	for (Component* script : mScripts)
	{
		if (script == nullptr)
			continue;

		script->Render();
	}
}

void GameObject::AddComponent(Component* comp)
{
	eComponentType order = comp->GetOrder();

	if (order == eComponentType::Script)
	{
		mScripts.push_back(comp);
		comp->SetOwner(this);
		return;
	}

	mComponents[(UINT)order] = comp;
	mComponents[(UINT)order]->SetOwner(this);
}
