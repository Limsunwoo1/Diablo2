#include "CEditorObject.h"

EditorObject::EditorObject()
	: GameObject()
{
}

EditorObject::~EditorObject()
{
}

void EditorObject::Initalize()
{
	GameObject::Initalize();
}

void EditorObject::Update()
{
	GameObject::Update();
}

void EditorObject::FixedUpdate()
{
	GameObject::FixedUpdate();
}
void EditorObject::Render()
{
	GameObject::Render();
}
