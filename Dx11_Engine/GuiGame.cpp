#include "GuiGame.h"
#include "Engin_Source/CResource.h"
#include "Engin_Source/CTexture2D.h"
#include "Engin_Source/CGraphicDevice_DX11.h"

#include "Engin_Source/CResourceManager.h"

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
		std::shared_ptr<graphics::Texture2D> renderTarget = ResourceManager::GetInstance()->Find<graphics::Texture2D>(L"RenderTarget");

		std::shared_ptr<graphics::Texture2D> gameTex = std::make_shared<graphics::Texture2D>();
		gameTex->Create(1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

		// 61 번 쉐이더 리소스 뷰 null 초기화
		ID3D11ShaderResourceView* gameSRV = nullptr;
		graphics::GetDevice()->BindShaderResource(graphics::eShaderStage::PS, 61, &gameSRV);
		graphics::GetDevice()->CopyResource(gameTex->GetTexture().Get(), renderTarget->GetTexture().Get());

		gameTex->BindShaderResource(graphics::eShaderStage::PS, 61);

		ImGuiIO io = ImGui::GetIO();
		ImVec2 panelSize = ImGui::GetWindowSize();
		ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec2 scene_Size = ImVec2(io.DisplaySize.x, io.DisplaySize.y);

	}
	void Game::LateUpdate()
	{
	}
}
