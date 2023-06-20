#include "CHpPotionItem.h"
#include "CResourceManager.h"
#include "CSpriteRenderer.h"
#include "CAnimator.h"
#include "CWorldManager.h"
#include "Cplayer.h"
#include "CItemManager.h"

HpPotionItem::HpPotionItem()
	: PotionItem()
{
	SetFill(250);
	SetItemType(eEquipmentType::HpPotion);
}

HpPotionItem::~HpPotionItem()
{
}

void HpPotionItem::Initalize()
{
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"HPPotionMaterial");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"HpPotion", L"Item//HpPotion.png");
	material.lock()->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	{
		Animator* animator = AddComponent<Animator>();
		weak_ptr<Texture2D> tex =
			ResourceManager::GetInstance()->Load<Texture2D>(L"HpPotionDrop", L"Item//HpPotionDrop.png");
		animator->Create(L"WorldDrop", tex, Vector2::Zero, Vector2(20.f, 159.f), Vector2::Zero, 17, 0.05f);
	}

	SetItemType(eEquipmentType::HpPotion);
}

void HpPotionItem::UsePotion()
{
	GameObject* object = GetTargetObject();
	if (object)
	{
		Player* player = dynamic_cast<Player*>(WorldManager::GetInstance()->GetPlayer());
		float hp = player->GetHP();
		float maxHp = player->GetMaxHP();

		hp += GetFill();
		if (hp > maxHp)
		{
			hp = maxHp;

		}

		player->SetHP(hp);
		// 오브젝트의 체력 회복
		// int hp = object->GetHp();
		// hp += GetFIll();
		// object->SetHP(hp);
	}

	ItemManager::GetInstance()->DeleteItem(this);
	this->Death();
}
