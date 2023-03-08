#pragma once
#include "CDebugObject.h"
#include "CWidget.h"
#include "CEditorObject.h"
#include "..//Engin_Source/Graphics.h"

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
	void DebugRender(graphics::DebugMesh& mesh);

	void GpuCameraMatrix(Camera* camera);
private:
	std::vector<Widget*> mWidgets;
	std::vector<EditorObject*> mEditorObjects;
	std::vector<DebugObject*> mDebugObjects;
};

