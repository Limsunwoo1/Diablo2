#include "CSkilSelectButton.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CUIManager.h"
#include "CSkilSelectPanel.h"
#include "CCurSelectSkilButton.h"

#include "CAudioClip.h"
#include "CAudioSource.h"

SkilSelectButton::SkilSelectButton()
	: Button(eUIType::Button)
	, mType(eSkilList::Attack)
{
	BindEvnet(std::bind(&SkilSelectButton::To_SkilSelectPanel, this));
	mMaterial = std::make_shared<Material>();

	std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"SpriteShader");

	mMaterial->SetShader(shader);
	mMaterial->SetRenderingMode(graphics::eRenderingMode::Transparent);

	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	sr->SetMesh(mesh);
	sr->SetMaterial(mMaterial);

	AudioSource* audio = AddComponent<AudioSource>();
	std::weak_ptr<AudioClip> clip = ResourceManager::GetInstance()->Load<AudioClip>(L"ButtonClick", L"SoundResource\\Effect\\button.wav");
	audio->SetClip(clip);
	audio->SetLoop(false);
}

SkilSelectButton::~SkilSelectButton()
{

}

void SkilSelectButton::InItTex(std::weak_ptr<Texture2D> tex)
{
	mMaterial->SetTexture(eTextureSlot::T0, tex);
}

void SkilSelectButton::To_SkilSelectPanel()
{
	//SkilSelect
	SkilSelectPanel* panel = UIManager::GetInstance()->GetUiInstance<SkilSelectPanel>(L"SkilSelect");
	panel->SetCurButton(this);
	panel->UnActive();

	//SkilSelectButton
	CurSelectSkilButton* button = UIManager::GetInstance()->GetUiInstance<CurSelectSkilButton>(L"SkilSelectButton");
	button->InItTex(mMaterial->GetTexture(eTextureSlot::T0));
	button->SetSkilType(this->GetSkilType());

	AudioSource* audio = GetComponent<AudioSource>();
	audio->Stop();
	audio->Play();
}