#include "CSkil.h"
#include "Cplayer.h"

Skil::Skil(Player* player)
	: GameObject()
	, mOwner(player)
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
