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

		void SetWidgetsPused(bool able = true);

		template<typename T>
		T* GetWidget(const std::string& name)
		{
			auto iter = mWidgets.find(name);
			if (iter == mWidgets.end())
				return nullptr;

			return dynamic_cast<T*>(iter->second);
		}

		UINT GetTileMaxX() { return TileMaxX; }
		UINT GetTileMaxY() { return TileMaxY; }

		void SetTileMaxX(UINT x) { TileMaxX = x; }
		void SetTileMaxY(UINT y) { TileMaxY = y; }

		UINT GetTileIndexX() { return TileIndexX; }
		UINT GetTileIndexY() { return TileIndexY; }

		void SetTileIndexX(UINT x) { TileIndexX = x; }
		void SetTileIndexY(UINT y) { TileIndexY = y; }

		void SetEditorWorldMousePos(Vector2 pos) { mEditorWorldMousePos = pos; }
		void SetEditorUiMousePos(Vector2 pos) { mEditorUiMousePos = pos; }
		void SetEditorImGuiMousePos(ImVec2 pos) { mImGuiMousePos = pos; }

		Vector2 GetEditorWorldMousePos() { return mEditorWorldMousePos; }
		Vector2 GetEditorUiMousePos() { return mEditorUiMousePos; }
		Vector2 GetImGuiMousePos() { return Vector2(mImGuiMousePos.x, mImGuiMousePos.y); }

		bool GetActive() {return !mbItemsAble;}
		bool SetActive(bool active = true) { mbItemsAble = active; }

	private:
		std::map<std::string ,Widget*> mWidgets;
		std::vector<EditorObject*> mEditorObjects;
		std::vector<DebugObject*> mDebugObjects;

		class DockEditor* mDockObject;
		EditorObject* mGrid;

		UINT TileMaxX;
		UINT TileMaxY;

		UINT TileIndexX;
		UINT TileIndexY;

		Vector2 mEditorWorldMousePos;
		Vector2 mEditorUiMousePos;
		ImVec2 mImGuiMousePos;

		bool mbItemsAble;
	};
}
