#include "CMpPotionItem.h"
#include "CItemManager.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"

MpPotionItem::MpPotionItem()
	: PotionItem()
{
	SetFill(20);
}

MpPotionItem::~MpPotionItem()
{
}

void MpPotionItem::Initalize()
{
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"MPPotionMaterial");
	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"MpPotion", L"Item//MpPotion.png");
	material->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);
}

void MpPotionItem::UsePotion()
{
	GameObject* object = GetTargetObject();
	if (object)
	{
		// 오브젝트의 마나 회복
		this->Death();

		if (GetSlotInventory() != nullptr)
		{
	
		}
	}
}
