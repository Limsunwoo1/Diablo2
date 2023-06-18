#include "CSkil.h"
#include "Cplayer.h"

Skil::Skil()
	: GameObject()
	, mCost(20.f)
{
}

Skil::Skil(Player* player)
	: GameObject()
	, mOwner(player)
	, mElementType(eElementType::None)
	, mCost(0.0f)
	, mDamege(20.f)
{
	
}

Skil::~Skil()
{
}

void Skil::Initalize()
{
	GameObject::Initalize();
}

void Skil::Update()
{
	GameObject::Update();
}

void Skil::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void Skil::Render()
{
	GameObject::Render();
}
