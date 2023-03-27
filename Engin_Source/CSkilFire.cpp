#include "CSkilFire.h"
#include "Cplayer.h"

SkilFire::SkilFire(Player* player)
	: GameObject()
	, mOwner(player)
{
	
}

SkilFire::~SkilFire()
{
}

void SkilFire::Initalize()
{
	GameObject::Initalize();
}

void SkilFire::Update()
{
	GameObject::Update();
}

void SkilFire::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void SkilFire::Render()
{
	GameObject::Render();
}
