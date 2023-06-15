#include "CShoesItem.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"

ShoesItem::ShoesItem(const wstring& name)
	: ItemBase(eEquipmentType::Shoes)
{
	SetItemName(name);
}

ShoesItem::~ShoesItem()
{
}

void ShoesItem::Initalize()
{
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"ItemShader");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"ShoesItem", L"Item//Shoes.png");
	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->SetShader(shader);
	material->SetTexture(eTextureSlot::T0, tex);

	sr->SetMesh(mesh);
	sr->SetMaterial(material);

	wstring name = L"Item";
	name += to_wstring(GetID());
	ResourceManager::GetInstance()->Insert<Material>(name, material);

	//shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(GetItemName());
	//material->SetTexture(eTextureSlot::T0, tex);

	{
		Animator* animator = AddComponent<Animator>();
		weak_ptr<Texture2D> tex =
			ResourceManager::GetInstance()->Load<Texture2D>(L"ShoesDrop", L"Item//ShoesDrop.png");
		animator->Create(L"WorldDrop", tex, Vector2::Zero, Vector2(28.f, 181.f), Vector2::Zero, 17, 0.05f);
	}

	SetItemSlotSize(Vector2(2.0f, 2.0f));
	SetItemMater(material.get());

	SetWorldSize(Vector3(4.0f * ITEM_X_SIZE * 2.0f, 4.0f * ITEM_Y_SIZE * 2.0f, 0.0f));
	SetInvenSize(Vector3(ITEM_X_SIZE * 2.0f, ITEM_Y_SIZE * 2.0f, 0.0f));
}
