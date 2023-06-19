#include "CTriggerObject.h"
#include "CWorldManager.h"
#include "Cplayer.h"

TriggerObject::TriggerObject()
	:GameObject()
{
	
}

TriggerObject::~TriggerObject()
{
}

void TriggerObject::Initalize()
{
}

void TriggerObject::Update()
{
	Player* player = dynamic_cast<Player*>(WorldManager::GetInstance()->GetPlayer());
	if (player == nullptr)
		return;

	Transform* playerTr = player->GetComponent<Transform>();
	Vector3 PlayerPos = playerTr->GetPosition();

	Transform* Tr = GetComponent<Transform>();
	Vector3 Pos = Tr->GetPosition();
	Vector3 Size = Tr->GetSize();

	if (PlayerPos.x > (Pos.x + Size.x * 0.5f) || PlayerPos.x < (Pos.x - Size.x * 0.5f))
		return;

	if (PlayerPos.y > (Pos.y + Size.y * 0.5f) || PlayerPos.y < (Pos.y - Size.y * 0.5f))
		return;

	if(mFunc)
		mFunc();


	Paused();
	GameObject::Update();
}

void TriggerObject::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void TriggerObject::Render()
{
}
