#include "CTelePort.h"
#include "CAnimator.h"
#include "CSpriteRenderer.h"
#include "CResourceManager.h"
#include "CObject.h"
#include "CWorldManager.h"

#include "CAudioClip.h"
#include "CAudioSource.h"

TelePort::TelePort()
	: Skil()
	, mMovePos(Vector3(-1.f, -1.f,-1.f))
{
}

TelePort::~TelePort()
{
}

void TelePort::Initalize()
{
	// Size
	Transform* tr = GetComponent<Transform>();
	tr->SetSize(Vector3(200.0f, 200.0f, 1.0f));

	// renderer
	SpriteRenderer* sr = AddComponent<SpriteRenderer>();
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Load<Texture2D>(L"TelePort", L"Light//telePort2.png");
	weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"TelePortMaterial");
	material.lock()->SetTexture(eTextureSlot::T0, tex);
	sr->SetMesh(mesh);
	sr->SetMaterial(material);
	
	InitAnimation();

	AudioSource* audio = AddComponent<AudioSource>();
	std::weak_ptr<AudioClip>clip = ResourceManager::GetInstance()->Load<AudioClip>(L"TelePortSound", L"SoundResource\\teleport.wav");
	audio->SetClip(clip);
	audio->SetLoop(false);


}

void TelePort::Update()
{
	Skil::Update();
}

void TelePort::FixedUpdate()
{
	if (mOwner)
	{
		Transform* ownerTr = mOwner->GetComponent<Transform>();
		Vector3 OwnerPos = ownerTr->GetPosition();
		Transform* mTr = GetComponent<Transform>();
		mTr->SetPosition(OwnerPos);

		Animator* animator = mOwner->GetComponent<Animator>();
		if (animator->GetPlayAnimation()->IsComplete())
		{
			GetComponent<AudioSource>()->Play(0.1f);
			if (mMovePos.x >= 0 && mMovePos.y >= 0)
			{
				ownerTr->SetPosition(mMovePos);
				Object::ObjectDestroy(this);
			}
		}
	}
	Skil::FixedUpdate();
}

void TelePort::Render()
{
	Skil::Render();
}

void TelePort::InitAnimation()
{
	Animator* animator = AddComponent<Animator>();
	weak_ptr<Texture2D> tex = ResourceManager::GetInstance()->Find<Texture2D>(L"TelePort");

	animator->Create(L"TelePort", tex, Vector2(0.0f, 0.0f), Vector2(136.f, 154.f), Vector2::Zero, 18, 0.03f);
	animator->Play(L"TelePort", false);
}
