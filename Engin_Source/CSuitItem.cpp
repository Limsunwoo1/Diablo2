#include "CSuitItem.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"

SuitItem::SuitItem(const wstring& name)
	: ItemBase(eEquipmentType::Suit)
{
	SetItemName(name);
}

SuitItem::~SuitItem()
{
}

void SuitItem::Initalize()
{
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"ItemShader");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"SuitItem", L"Item//Suit.png");
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
			ResourceManager::GetInstance()->Load<Texture2D>(L"SuitDrop", L"Item//Suit2Drop.png");
		animator->Create(L"WorldDrop", tex, Vector2::Zero, Vector2(38.f, 173.f), Vector2::Zero, 17, 0.05f);
	}

	SetItemSlotSize(Vector2(2.0f, 3.0f));
	SetItemMater(material.get());

	SetWorldSize(Vector3(4.0f * ITEM_X_SIZE * 2.0f, 4.0f * ITEM_Y_SIZE * 2.0f, 1.0f));
	SetInvenSize(Vector3(ITEM_X_SIZE * 2.0f, ITEM_Y_SIZE * 3.0f, 1.0f));
}
