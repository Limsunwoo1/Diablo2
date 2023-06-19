#include "CBeltItem.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"

BeltItem::BeltItem(const wstring& name)
	: ItemBase(eEquipmentType::Belt)
{
	SetItemName(name);
}

BeltItem::~BeltItem()
{
}

void BeltItem::Initalize()
{
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"ItemShader");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"BeltItem", L"Item//Belt.png");
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
			ResourceManager::GetInstance()->Load<Texture2D>(L"BeltDrop", L"Item//BeltDrop.png");
		animator->Create(L"WorldDrop", tex, Vector2::Zero, Vector2(15.f, 154.f), Vector2::Zero, 17, 0.05f);
	}

	SetItemSlotSize(Vector2(2.0f, 1.0f));
	SetItemMater(material.get());

	SetWorldSize(Vector3(5.0f * ITEM_X_SIZE * 2.0f, 5.0f * ITEM_Y_SIZE * 2.0f, 0.0f));
	SetInvenSize(Vector3(ITEM_X_SIZE * 2.0f, ITEM_Y_SIZE * 1.0f, 0.0f));
	
	ItemBase::Initalize();
}
