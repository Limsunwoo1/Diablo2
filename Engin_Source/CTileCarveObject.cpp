#include "CTileCarveObject.h"
#include "CMaterial.h"
#include "CResourceManager.h"
#include "CBaseRenderer.h"
#include "CShader.h"

TileCarveObject::TileCarveObject()
	: TileObject()
{
	
}

TileCarveObject::~TileCarveObject()
{
}

void TileCarveObject::Initalize()
{
	std::weak_ptr<Material> mCarveMater = ResourceManager::GetInstance()->Find<Material>(L"CarveMaterial");
	mCarveMater.lock()->SetTexture(eTextureSlot::T0, GetMaterial()->GetTexture(eTextureSlot::T0));

	SetMaterial(mCarveMater.lock().get());

	BaseRenderer* renderer = this->GetComponent<BaseRenderer>();
	renderer->SetMaterial(mCarveMater);
}
