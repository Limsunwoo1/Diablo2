#include "CGloveItem.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"

GloveItem::GloveItem(const wstring& name)
	: ItemBase(eEquipmentType::Glove)
{
	SetItemName(name);
}

GloveItem::~GloveItem()
{
}

void GloveItem::Initalize()
{
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	shared_ptr<Shader> shader = ResourceManager::GetInstance()->Find<Shader>(L"ItemShader");
	shared_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"GloveItem", L"Item//Glove.png");
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

	SetItemSlotSize(Vector2(2.0f, 2.0f));
	SetItemMater(material.get());
}
