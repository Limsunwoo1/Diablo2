#pragma once
#include "GuiDebugObject.h"
#include "GuiWidget.h"
#include "GuiEditorObject.h"
#include "..//Engin_Source/Graphics.h"
#include "..//Engin_Source/CApplication.h"
#include "..//Engin_Source/CGraphicDevice_DX11.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

extern CApplication Application;

class Camera;
namespace gui
{
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

		void ImGui_Initalize();
		void ImGui_Run();
		void ImGui_Release();
	private:
		std::vector<Widget*> mWidgets;
		std::vector<EditorObject*> mEditorObjects;
		std::vector<DebugObject*> mDebugObjects;
	};
}
