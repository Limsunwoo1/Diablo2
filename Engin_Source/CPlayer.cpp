#include "Cplayer.h"
#include "CResourceManager.h"
#include "CSpriteRenderer.h"
#include "CMaterial.h"
#include "CTexture2D.h"

Player::Player()
	: GameObject()
	, mHP(10)
	, mMP(10)
	, mDamege(5)
{

}

Player::~Player()
{

}

void Player::Initalize()
{
	GameObject::Initalize();
}

void Player::Update()
{
	GameObject::Update();
}

void Player::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void Player::Render()
{
	SpriteRenderer* spr = GetComponent<SpriteRenderer>();
	std::shared_ptr<Texture2D> texture = ResourceManager::GetInstance()->Find<Texture2D>(L"test");
	spr->GetMaterial()->SetTexture(texture);


	GameObject::Render();
}
