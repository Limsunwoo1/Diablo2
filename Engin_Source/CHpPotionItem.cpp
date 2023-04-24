#include "CHpPotionItem.h"
#include "CResourceManager.h"
#include "CSpriteRenderer.h"

HpPotionItem::HpPotionItem()
	: PotionItem()
{
	SetFill(20);
}

HpPotionItem::~HpPotionItem()
{
}

void HpPotionItem::Initalize()
{
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"HPPotionMaterial");
	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"HpPotion", L"Item//HpPotion.png");
	material->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);
}

void HpPotionItem::UsePotion()
{
	GameObject* object = GetTargetObject();
	if (object)
	{
		// 오브젝트의 체력 회복
		// int hp = object->GetHp();
		// hp += GetFIll();
		// object->SetHP(hp);
	}
	this->Death();
}
