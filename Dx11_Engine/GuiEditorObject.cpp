#include "GuiEditorObject.h"

#include "..//Engin_Source/CSceneManager.h"
#include "..//Engin_Source/CScene.h"
namespace gui
{
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
		/*if (SceneManager::GetInstance()->GetActiveScene()->GetScenType() != eSceneType::Tool)
			return;*/

		GameObject::Update();
	}

	void EditorObject::FixedUpdate()
	{
	/*	if (SceneManager::GetInstance()->GetActiveScene()->GetScenType() != eSceneType::Tool)
			return;*/
		GameObject::FixedUpdate();
	}
	void EditorObject::Render()
	{
		if (SceneManager::GetInstance()->GetActiveScene()->GetScenType() != eSceneType::Tool)
			return;
		GameObject::Render();
	}
}