#include "CMpPotionItem.h"
#include "CItemManager.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CAnimator.h"

MpPotionItem::MpPotionItem()
	: PotionItem()
{
	SetFill(20);
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

}

void MpPotionItem::UsePotion()
{
	GameObject* object = GetTargetObject();
	if (object)
	{
		// 오브젝트의 마나 회복

		if (GetSlotInventory() != nullptr)
		{
	
		}
	}
	this->Death();
}
