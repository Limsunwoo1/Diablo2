#include "CMpPotionItem.h"
#include "CItemManager.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "Cplayer.h"
#include "CWorldManager.h"
#include "CItemManager.h"

#include "CAudioClip.h"
#include "CAudioSource.h"
#include "CResourceManager.h"


MpPotionItem::MpPotionItem()
	: PotionItem()
{
	SetFill(150);
	SetItemType(eEquipmentType::MpPotion);
}

MpPotionItem::~MpPotionItem()
{
}

void MpPotionItem::Initalize()
{
	{
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"MPPotionMaterial");
		weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"MpPotion", L"Item//MpPotion.png");
		material.lock()->SetTexture(eTextureSlot::T0, tex);

		sr->SetMesh(mesh);
		sr->SetMaterial(material);
	}

	{	
		Animator* animator = AddComponent<Animator>();
		weak_ptr<Texture2D> tex =
			ResourceManager::GetInstance()->Load<Texture2D>(L"MpPotionDrop", L"Item//MpPotionDrop.png");
		animator->Create(L"WorldDrop", tex, Vector2(0.0f, 0.0f), Vector2(20.f, 159.f), Vector2(0.0f, 0.0f), 17, 0.05f);
	}

	SetItemType(eEquipmentType::HpPotion);

}

void MpPotionItem::UsePotion()
{
	GameObject* object = GetTargetObject();
	if (object)
	{
		Player* player = dynamic_cast<Player*>(WorldManager::GetInstance()->GetPlayer());
		if (player == nullptr)
			return;


		std::weak_ptr<AudioClip> clip = ResourceManager::GetInstance()->Load<AudioClip>(L"PotionDrink", L"SoundResource\\Effect\\potiondrink.wav");

		AudioSource* audio = GetComponent<AudioSource>();
		audio->Stop();
		audio->SetClip(clip);
		audio->SetLoop(false);
		audio->Play();


		float mp = player->GetMP();
		float maxmp = player->GetMaxMP();

		mp += GetFill();
		if (mp > maxmp)
		{
			mp = maxmp;

		}

		player->SetMP(mp);
	}

	ItemManager::GetInstance()->DeleteItem(this);
	this->Paused();
}
