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

#include <iostream>

extern CApplication Application;

namespace gui
{
	Game::Game()
	{
		SetName("Game");
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

		this->WindowFocus(true);
		ImVec2 window = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 mouse = ImGui::GetMousePos();

		ImVec2 left = mouse - window;
		ImVec2 Pos = (mouse - window) /  windowSize;
		
		if (Pos.x < 0 || Pos.y < 0)
			return;
		
		UINT width = Application.GetWidth();
		UINT height = Application.GetHeight();
		Vector2 mousePos = Input::GetInstance()->GetMouseWorldPos(Vector2(Pos.x * width, Pos.y * height));

		std::cout << mousePos.x << "      X" << mousePos.y << "       Y" << std::endl;
		if (Input::GetInstance()->GetKeyDown(eKeyCode::LCTRL))
		{
			GameObject* object = Object::Instantiate<GameObject>(eLayerType::Player, true);
			object->GetComponent<Transform>()->SetPosition(Vector3(mousePos.x, mousePos.y, 0.0f));
			MeshRenderer* mr = object->AddComponent<MeshRenderer>();
			std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
			std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");

			mr->SetMesh(mesh);
			mr->SetMaterial(material);
		}

		this->WindowFocus(false);
	}
	void Game::LateUpdate()
	{
	}
}
