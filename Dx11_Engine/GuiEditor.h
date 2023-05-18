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

		template<typename T>
		T* GetWidget(const std::string& name)
		{
			auto iter = mWidgets.find(name);
			if (iter == mWidgets.end())
				return nullptr;

			return dynamic_cast<T*>(iter->second);
		}
	private:
		std::map<std::string ,Widget*> mWidgets;
		std::vector<EditorObject*> mEditorObjects;
		std::vector<DebugObject*> mDebugObjects;

		class DockEditor* mDockObject;
	};
}
