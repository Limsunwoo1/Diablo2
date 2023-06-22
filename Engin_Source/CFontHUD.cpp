#include "CFontHUD.h"
#include "CRenderer.h"
#include "CConstantBuffer.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CTime.h"

FontHUD::FontHUD()
	: HUD(eUIType::HUD)
	, mCurNum(0)
	, mMaxNum(10)
{

}

FontHUD::~FontHUD()
{

}

void FontHUD::Initalize()
{
	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Material> mater = ResourceManager::GetInstance()->Find<Material>(L"FontMaterial");
	std::weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"Font", L"..//Resource//font1.png");

	mater.lock()->SetTexture(eTextureSlot::T0, tex);
	mater.lock().get()->SetRenderingMode(graphics::eRenderingMode::Transparent);

	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	sr->SetMesh(mesh);
	sr->SetMaterial(mater);
}

void FontHUD::Render()
{
	float interver = 1.f / (float)mMaxNum;

	Renderer::FadeCB info = {};
	info.ImageStart = Vector2((interver * mCurNum), 0.0f);
	info.ImageEnd = Vector2((interver * mCurNum) + interver, 1.0f);

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Fade];
	cb->SetData(&info);
	cb->Bind(eShaderStage::PS);


	HUD::Render();

}