#include "GuiGame.h"
#include "Engin_Source/CResource.h"
#include "Engin_Source/CTexture2D.h"
#include "Engin_Source/CGraphicDevice_DX11.h"

#include "Engin_Source/CResourceManager.h"
#include "..//Engin_Source/CInput.h"
#include "..//Engin_Source/CObject.h"

#include "..//Engin_Source/CMeshRenderer.h"
#include "..//Engin_Source/CApplication.h"

#include "..//Engin_Source/CGraphicDevice_DX11.h"
#include "..//Engin_Source/CTileObject.h"
#include "..//Engin_Source/CTexture2D.h"
#include "..//Engin_Source/CWallObject.h"
#include "..//Engin_Source/CLavaTile.h"

#include "..//Engin_Source/CToolScene.h"
#include "..//Engin_Source/CObjectManager.h"

#include "GuiEditor.h"
#include "GuiInspector.h"
#include "GuiHierachy.h"

#include <iostream>

extern CApplication Application;
extern gui::Editor _Editor;

namespace gui
{
	Game::Game()
	{
		SetName("Game");
		mbCreateTile = true;
		mbCreateObject = true;
		mbTickOff = false;
	}
	Game::~Game()
	{
	}
	void Game::FixedUpdate()
	{
	}
	void Game::Update()
	{
		std::weak_ptr<graphics::Texture2D> renderTarget = ResourceManager::GetInstance()->Find<graphics::Texture2D>(L"RenderTarget");

		std::shared_ptr<graphics::Texture2D> gameTex = std::make_shared<graphics::Texture2D>();
		gameTex->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

		// 61 번 쉐이더 리소스 뷰 null 초기화
		ID3D11ShaderResourceView* gameSRV = nullptr;
		graphics::GetDevice()->BindShaderResource(graphics::eShaderStage::PS, 61, &gameSRV);
		graphics::GetDevice()->CopyResource(gameTex->GetTexture().Get(), renderTarget.lock()->GetTexture().Get());

		gameTex->BindShaderResource(graphics::eShaderStage::PS, 61);

		ImGuiIO io = ImGui::GetIO();
		ImVec2 panelSize = ImGui::GetWindowSize();
		ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec2 scene_Size = ImVec2(io.DisplaySize.x, io.DisplaySize.y);

		ImGui::Image((void*)gameTex->GetSRV().Get(),
							panelSize,
							ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
							tintColor);

		ImVec2 window = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 mouse = ImGui::GetMousePos();

		//ImVec2 client = ImGui::GetContentRegionAvail();


		ImVec2 Pos = (mouse - window) /  windowSize;
		//mouse += window;
	

		if (Pos.x < 0 || Pos.y < 0)
		{
			return;
		}
	
		UINT width = Application.GetWidth();
		UINT height = Application.GetHeight();
		Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(Vector2(Pos.x * width, Pos.y * height));
		Vector2 uiMousePos = Input::GetInstance()->GetMouseWorldPos(Renderer::UiCamera,Vector2(Pos.x * width, Pos.y * height));
		_Editor.SetEditorUiMousePos(uiMousePos);
		_Editor.SetEditorImGuiMousePos(mouse);
		_Editor.SetEditorWorldMousePos(mousePos);

		if (SceneManager::GetInstance()->GetActiveScene()->GetScenType() != eSceneType::Tool)
			return;

		if (mouse.x < window.x || window.x + windowSize.x < mouse.x)
			return;

		if (mouse.y < window.y || window.y + windowSize.y < mouse.y)
			return;

		/*if (mTex == nullptr)
			return;*/

		ToolScene* toolscene = dynamic_cast<ToolScene*>(SceneManager::GetInstance()->GetActiveScene());
		eToolRenderMode rMode = toolscene->GetToolRenderMode();

		if (Input::GetInstance()->GetKeyPress(eKeyCode::LCTRL))
		{
			if (Input::GetInstance()->GetKeyPress(eKeyCode::LBTN)
				&& mbCreateTile
				&& mTex->GetName().find(L"Object") == wstring::npos
				&& mTex->GetName().find(L"object") == wstring::npos
				&& rMode == eToolRenderMode::TILE
				&& mTex != nullptr)
			{

				int x = Input::GetInstance()->GetIsometricX();
				int y = Input::GetInstance()->GetIsometricY();

				if (x < 0 || y < 0)
					return;

				TileObject* object = Object::Instantiate<TileObject>(eLayerType::Tile, true);

				Transform* objectTr = object->GetComponent<Transform>();
				int Xpos = (x - y) * TILE_X_HALF_SIZE;
				int ypos = (x + y) * TILE_Y_HALF_SIZE;

				objectTr->SetPosition(Vector3(Xpos + 5000.f, ypos + 5000.f, 50.f));
				objectTr->SetSize(Vector3(400.f, 200.f, 1.0f));

				object->SetMaxIndex(_Editor.GetTileMaxX(), _Editor.GetTileMaxY());
				object->SetIndex(_Editor.GetTileIndexX(), _Editor.GetTileIndexY());
				object->SetScreenIndex(x, y);

				MeshRenderer* mr = object->AddComponent<MeshRenderer>();
				std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
				std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"TileMaterial");

				// test
				if (mTex != nullptr)
				{
					std::weak_ptr<graphics::Texture2D> tex = ResourceManager::GetInstance()->Find<graphics::Texture2D>(mTex->GetName());
					material.lock()->SetTexture(eTextureSlot::T0, tex);
				}

				mr->SetMesh(mesh);
				mr->SetMaterial(material);

				ObjectManager::GetInstance()->InsertTileObject(object);
			}

			if (Input::GetInstance()->GetKeyPress(eKeyCode::LBTN)
				&& mbCreateObject
				&& mTex->GetName().find(L"Object") != wstring::npos
				&& rMode == eToolRenderMode::OBJECT
				&& mTex != nullptr)
			{
				int x = Input::GetInstance()->GetIsometricX();
				int y = Input::GetInstance()->GetIsometricY();

				if (x < 0 || y < 0)
					return;

				WallObject* object = Object::Instantiate<WallObject>(eLayerType::Wall, true);

				Transform* objectTr = object->GetComponent<Transform>();
				int Xpos = (x - y) * TILE_X_HALF_SIZE;
				int ypos = (x + y) * TILE_Y_HALF_SIZE;

				objectTr->SetPosition(Vector3(Xpos + 5000.f, ypos + 5000.f, 49.f));
				objectTr->SetSize(Vector3(300.f, 300.f, 1.0f));

				object->SetTexture(ResourceManager::GetInstance()->Find<Texture2D>(mTex->GetName()));
				object->SetScrrenIndex(x, y);

				ObjectManager::GetInstance()->InsertWallObject(object);
			}

		}

		if (Input::GetInstance()->GetKeyPress(eKeyCode::SPACE))
		{
			if (Input::GetInstance()->GetKeyPress(eKeyCode::LBTN)
				&& mbCreateTile
				&& rMode == eToolRenderMode::TILE)
			{

				int x = Input::GetInstance()->GetIsometricX();
				int y = Input::GetInstance()->GetIsometricY();

				if (x < 0 || y < 0)
					return;

				LavaTile* object = Object::Instantiate<LavaTile>(eLayerType::Tile, true);

				Transform* objectTr = object->GetComponent<Transform>();
				int Xpos = (x - y) * TILE_X_HALF_SIZE;
				int ypos = (x + y) * TILE_Y_HALF_SIZE;

				objectTr->SetPosition(Vector3(Xpos + 5000.f, ypos + 5000.f, 50.f));
				objectTr->SetSize(Vector3(400.f, 200.f, 1.0f));

				object->SetScreenIndex(x, y);

				ObjectManager::GetInstance()->InsertTileObject(object);
			}
		}

		mbCreateTile = true;
		mbCreateObject = true;
	}
	void Game::LateUpdate()
	{
	}
}
