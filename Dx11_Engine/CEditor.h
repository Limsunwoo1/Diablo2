#pragma once
#include "..//Editor_Source/CDebugObject.h"
#include "..//Editor_Source/CWidget.h"
#include "..//Editor_Source/CEditorObject.h"

class Editor
{
public:
	Editor();
	~Editor();

	void Initalize();
	void Run();

	void Update();
	void FixedUpdate();
	void Render();
	void Release();
	void DebugRender();

private:
	std::vector<Widget> mWidgets;
	std::vector<EditorObject> mEditorObjects;
	std::vector<DebugObject> mDebugObjects;
};

