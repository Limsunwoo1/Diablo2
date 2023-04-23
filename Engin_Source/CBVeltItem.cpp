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
	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"ItemShader");
	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"BeltItem", L"Item//Belt.png");
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

	SetItemSlotSize(Vector2(2.0f, 1.0f));
	SetItemMater(material.get());
}
