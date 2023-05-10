#include "CCapItem.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"

CapItem::CapItem(const wstring& name)
	: ItemBase(eEquipmentType::Cap)
{
	SetItemName(name);
}

CapItem::~CapItem()
{
}

void CapItem::Initalize()
{
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"ItemShader");
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"CapItem", L"Item//Cap.png");
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
			ResourceManager::GetInstance()->Load<Texture2D>(L"CapDrop", L"Item//CapDrop.png");
		animator->Create(L"WorldDrop", tex, Vector2::Zero, Vector2(33.f, 162.f), Vector2::Zero, 17, 0.05f);
	}

	SetItemSlotSize(Vector2(2.0f, 2.0f));
	SetItemMater(material.get());

	SetWorldScale(Vector3(4.0f, 4.0f, 0.0f));
	SetInvenScale(Vector3(ITEM_X_SIZE * 2.0f, ITEM_Y_SIZE * 2.0f, 0.0f));
}
