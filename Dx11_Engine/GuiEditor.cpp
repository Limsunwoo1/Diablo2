#include "GuiEditor.h"

#include "..//Engin_Source/CResourceManager.h"
#include "..//Engin_Source/CMesh.h"
#include "..//Engin_Source/CMaterial.h"
#include "..//Engin_Source/CMeshRenderer.h"
#include "..//Engin_Source/CGridScript.h"
#include "..//Engin_Source/CObject.h"

#include "..//Engin_Source/CRenderer.h"

#include "..//Engin_Source/CInput.h"

#include "GuiInspector.h"
#include "GuiHierachy.h"
#include "GuiProject.h"
#include "GuiDockEditor.h"
#include "GuiConsol.h"
#include "GuiGame.h"
#include "GuiListWidget.h"

namespace gui
{
	Editor::Editor()
		: mbItemsAble(false)
	{
	}

	Editor::~Editor()
	{
	}

	void Editor::Initalize()
	{
		// 충돌체의 종류 갯수만큼만 있으면 된다
		mDebugObjects.resize((UINT)eColliderType::End);

		// Rect
		std::weak_ptr<Mesh> rectMesh = ResourceManager::GetInstance()->Find<Mesh>(L"DebugRectMesh");
		std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"DebugMaterial");

		mDebugObjects[(UINT)eColliderType::Rect] = new DebugObject();
		MeshRenderer* meshRenderer = mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh(rectMesh);
		meshRenderer->SetMaterial(material);

		// Circle
		std::weak_ptr<Mesh> circleMesh = ResourceManager::GetInstance()->Find<Mesh>(L"CircleMesh");

		mDebugObjects[(UINT)eColliderType::Circle] = new DebugObject();

		MeshRenderer* circleRenderer = mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<MeshRenderer>();
		circleRenderer->SetMesh(circleMesh);
		circleRenderer->SetMaterial(material);


		// Grid
		EditorObject* girdObject = new EditorObject();
		girdObject->SetName(L"GridObject");
		MeshRenderer* gridMr = girdObject->AddComponent<MeshRenderer>();
		gridMr->SetMesh(ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh"));
		gridMr->SetMaterial(ResourceManager::GetInstance()->Find<Material>(L"GridMaterial"));
		GridScript* gridScript = girdObject->AddComponent<GridScript>();

		mEditorObjects.push_back(girdObject);
		mGrid = girdObject;

		ImGui_Initalize();

		mDockObject = new DockEditor();
		// mWidgets.push_bakc(mDockEditor)
		 
		// Init Widget
		Inspector* inspector = new Inspector();
		mWidgets.insert(std::make_pair("Inspector", inspector));

		Game* game = new Game();
		mWidgets.insert(std::make_pair("Game", game));
		
		Hierachy* hierachy = new Hierachy();
		mWidgets.insert(std::make_pair("hierachy", hierachy));

		Project* project = new Project();
		mWidgets.insert(std::make_pair("project", project));

		Consol* consol = new Consol();
		mWidgets.insert(std::make_pair("consol", consol));

	/*	ListWidget* listWidget = new ListWidget();
		mWidgets.insert(std::make_pair("ListWidget", listWidget));*/
	}

	void Editor::Run()
	{
		Update();
		FixedUpdate();
		Render();

		ImGui_Run();
	}

	void Editor::Update()
	{
		if (Input::GetInstance()->GetKeyDown(eKeyCode::LSHIFT))
		{
			mbItemsAble = (mbItemsAble == true ? false : true);
			SetWidgetsPused(mbItemsAble);
		}
		//mGridSc->SetCamera(Renderer::mainCamera);
		for (EditorObject* obj : mEditorObjects)
		{
			if (!obj)
				continue;

			obj->Update();
		}
	}

	void Editor::FixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			if (!obj)
				continue;

			obj->FixedUpdate();
		}

		Vector3 cameraPos = Renderer::mainCamera->GetOwner()->GetComponent<Transform>()->GetPosition();
		mGrid->GetComponent<Transform>()->SetPosition(cameraPos);
	}

	void Editor::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			if (!obj)
				continue;

			obj->Render();
		}

		for (DebugMesh& mesh : Renderer::debugMeshes)
		{

			DebugRender(mesh);
		}

		Renderer::debugMeshes.clear();
	}

	void Editor::Release()
	{
		ImGui_Release();

		for (auto iter : mWidgets)
		{
			if (iter.second == nullptr)
				continue;

			delete iter.second;
			iter.second = nullptr;
		}
		
		if (mDockObject)
		{
			delete mDockObject;
			mDockObject = nullptr;
		}

		for (auto* obj : mEditorObjects)
		{
			if (!obj)
				continue;

			delete obj;
			obj = nullptr;
		}

		delete mDebugObjects[(UINT)eColliderType::Rect];
		delete mDebugObjects[(UINT)eColliderType::Circle];
	}

	void Editor::DebugRender(graphics::DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];

		Transform* tr = debugObj->GetComponent<Transform>();
		mesh.position.z = -100.f;
		tr->SetPosition(mesh.position); 
	
		//tr->SetRotation(Vector3(XM_2PI / 60.f, XM_2PI / 60.f, mesh.rotation.z));

		// 레이어별 차이
		if (mesh.type == eColliderType::Rect)
			tr->SetSize(mesh.scale);
		else if (mesh.type == eColliderType::Circle)
			tr->SetSize(mesh.scale);


		BaseRenderer* renderer = debugObj->GetComponent<BaseRenderer>();

		tr->FixedUpdate();

		// 오브젝트의 종류에 따라 해당 카메라 세팅
		Camera* camera = Renderer::mainCamera;
		if (mesh.LayerType == (UINT)eLayerType::UI)
			camera = Renderer::UiCamera;

		// ui 가 렌더되지않을때 충돌체도 그려지지 않는다
		if (mesh.LayerType == (UINT)eLayerType::UI && !mesh.renderAble)
			return;

		// 마우스가 버튼위에 올라갔을때 색변화
		int i = mesh.uiState;
		renderer->GetMaterial().lock()->SetData(eGpuParam::Int, &i);

		GpuCameraMatrix(camera);

		debugObj->Render();
	}

	void Editor::GpuCameraMatrix(Camera* camera)
	{
		Camera::SetGpuViewMatrix(camera->GetViewMatrix());
		Camera::SetGpuProjectionMatrix(camera->GetProjectionMatrix());
	}

	void Editor::ImGui_Initalize()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;
		//io.ConfigViewportsNoDefaultParent = true;
		//io.ConfigDockingAlwaysTabBar = true;
		//io.ConfigDockingTransparentPayload = true;
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
		//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(Application.GetHwnd());
		ImGui_ImplDX11_Init(graphics::GetDevice()->GetID3D11Device()
			, graphics::GetDevice()->GetID3D11DeviceContext());

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
		// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
		// - Read 'docs/FONTS.md' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != nullptr);

		// Our state
	}

	void Editor::ImGui_Run()
	{
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		ImGuiIO io = ImGui::GetIO();

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		/*if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);*/


		mDockObject->Render();
		for (auto widget : mWidgets)
		{
			widget.second->Render();
		}

#pragma region SamPle


		//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		//{
		//	static float f = 0.0f;
		//	static int counter = 0;

		//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//	ImGui::Checkbox("Another Window", &show_another_window);

		//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//		counter++;
		//	ImGui::SameLine();
		//	ImGui::Text("counter = %d", counter);

		//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		//	ImGui::End();
		//}

		//// 3. Show another simple window.
		//if (show_another_window)
		//{
		//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//	ImGui::Text("Hello from another window!");
		//	if (ImGui::Button("Close Me"))
		//		show_another_window = false;
		//	ImGui::End();
		//}
#pragma endregion

		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


		// Update and Render additional Platform Windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void Editor::ImGui_Release()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void Editor::SetWidgetsPused(bool able)
	{
		if (able)
		{
			mDockObject->SetState(gui::Widget::eState::Paused);
			for (auto widget : mWidgets)
			{
				if (widget.second == nullptr)
					continue;

				widget.second->SetState(gui::Widget::eState::Paused);
			}
		}
		else
		{
			mDockObject->SetState(gui::Widget::eState::Active);
			for (auto widget : mWidgets)
			{
				if (widget.second == nullptr)
					continue;

				widget.second->SetState(gui::Widget::eState::Active);
			}
		}
	}
}
