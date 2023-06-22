#include "CItemBase.h"
#include "CInput.h"
#include "CCollider2D.h"
#include "CUIManager.h"
#include "CWorldManager.h"
#include "CAnimator.h"
#include "CInventoryPanel.h"
#include "CInventoryButton.h"
#include "CItemManager.h"
#include "CCollider2D.h"
#include "CCollisionManager.h"

#include "CSceneManager.h"
#include "CAudioClip.h"
#include "CAudioSource.h"
#include "CResourceManager.h"

ItemBase::ItemBase(eEquipmentType type)
	: GameObject()
	, mType(type)
	, mXSize(0)
	, mYSize(0)
	, mXIndex(0)
	, mYIndex(0)
	, mbStage(false)
	, mbPick(false)
	, mbOnInvnetory(false)
	, mbDrop(false)
	, mMaterial(nullptr)
	, mInventory(nullptr)
	, mSlotInventory(nullptr)
	, mTargetObject(nullptr)
{

}

ItemBase::~ItemBase()
{
}

void ItemBase::Initalize()
{
	Transform* tr = GetComponent<Transform>();
	Vector3 Pos = tr->GetPosition();
	float offsetY = Pos.y - (mWorldScale.y * 0.8f);



	Collider2D* col = AddComponent<Collider2D>();
	col->SetType(eColliderType::Rect);
	col->SetCenter(Vector2(0, -(mWorldScale.y * 0.35f)));
	col->SetSize(Vector2(0.1f, 0.1f));

	AudioSource* source = AddComponent<AudioSource>();
	std::weak_ptr<AudioClip> clip = ResourceManager::GetInstance()->Load<AudioClip>(L"ItemDrop", L"SoundResource\\Effect\\flippy.wav");

	source->SetClip(clip);
	source->SetLoop(false);
}

void ItemBase::Update()
{
	GameObject::Update();

	Vector2	mousePos = Input::GetInstance()->GetMouseWorldPos(mbStage);

	Transform* tr = GetComponent<Transform>();
	Vector3 colPos = tr->GetPosition();
	Vector3 colScale = tr->GetScale() * tr->GetSize();

	// �����ۿ� ���콺�� �浹 üũ
	if (mbStage == false)
	{
		if (mousePos.x > colPos.x + (colScale.x * 0.5f) || mousePos.x < colPos.x - (colScale.x * 0.5f))
			return;
		if (mousePos.y > colPos.y + (colScale.x * 0.5f) || mousePos.y < colPos.y - (colScale.y * 0.5f))
			return;
	}
	else
	{
		bool collision = CollisionManager::GetInstance()->AABBRect_VS_Point(GetComponent<Collider2D>(), mousePos);
		if (!collision)
			return;
	}

	if (Input::GetInstance()->GetKeyDown(eKeyCode::LBTN))
	{
		if (mbStage == true)
		{
			Panel* inventory = UIManager::GetInstance()->GetUiInstance<Panel>(L"mainInventory");
			bool able = inventory->GetIsAble();

			if (able == false)
				return;
		}
		// ���콺�� �ٸ� �������� ���� �ʰ� �ִ� ����
		if (Input::GetInstance()->GetPickItem() == nullptr)
		{
			SetStage(false);

			Transform* tr = this->GetComponent<Transform>();
			tr->SetSize(this->GetInvenSize());

			mbPick = true;
			Input::GetInstance()->SetPickItem(this);

			Animator* animator = GetComponent<Animator>();

			if (animator != nullptr)
			{
				animator->StopPlayAnimation();
			}

			// ���ĭ�� ���� �κ��丮���� �Ǻ��Ͽ�
			// �˸��� ��ġ�� ������ش�
			if (mSlotInventory == nullptr && mInventory != nullptr)
				mInventory->ClearPocketSlot(this);
			else if (mSlotInventory != nullptr)
				mSlotInventory->ClearPocketSlot(this);
			else
				int a = 0;
		}
		// ���콺�� �������� �����ٰ� ���� ���� �������� ������ �ִ� ��Ȳ���� üũ �����̳� �κ��丮���� üũ���ش�
		else if(mbDrop && Input::GetInstance()->GetPickItem() == this)
		{
			SetStage(false);

			Transform* tr = this->GetComponent<Transform>();
			tr->SetSize(this->GetInvenSize());

			mbPick = false;
			Input::GetInstance()->SetPickItem(nullptr);

			// ���ĭ�� ���� �κ��丮���� �Ǻ��Ͽ�
			// �˸��� ��ġ�� ������ش�
			if (mSlotInventory == nullptr && mInventory != nullptr)
			{
				mInventory->DropItem(this);
			}
			else if (mSlotInventory != nullptr)
			{
				mSlotInventory->DropItem(this);
			}
			else
			{
				mbDrop = false;
				mbPick = true;

				Animator* animator = GetComponent<Animator>();

				if (animator != nullptr)
				{
					animator->StopPlayAnimation();
				}

				Input::GetInstance()->SetPickItem(this);
			}
		}
		else
		{
			if (Input::GetInstance()->GetPickItem() != this)
				return;

			if (this->GetInventory() || this->GetSlotInventory())
				return;

			UiBase* mainPanel = UIManager::GetInstance()->GetUiInstance<UiBase>(L"mainPanel");
			UiBase* InventoryPanel = UIManager::GetInstance()->GetUiInstance<UiBase>(L"mainInventory");

			Transform* mainTr = mainPanel->GetComponent<Transform>();
			Vector3 mainPos = mainTr->GetPosition();
			Vector3 mainScale = mainTr->GetScale() * mainTr->GetSize();

			Transform* InvenTr = InventoryPanel->GetComponent<Transform>();
			Vector3 InvenPos = InvenTr->GetPosition();
			Vector3 InvenScale = InvenTr->GetScale() * InvenTr->GetSize();

			Transform* ItemTr = this->GetComponent<Transform>();
			Vector3 ItemPos = ItemTr->GetPosition();
			Vector3 ItemScale = ItemTr->GetScale()* ItemTr->GetSize();

			mbPick = true;
			if (mainPos.x - (mainScale.x * 0.5f) <= ItemPos.x && mainPos.x + (mainScale.x * 0.5f) >= ItemPos.x
				&& mainPos.y - (mainScale.y * 0.5f) <= ItemPos.y && mainPos.y + (mainScale.y * 0.5f) >= ItemPos.y)
			{
				mbDrop = false;
				Input::GetInstance()->SetPickItem(this);

				Animator* animator = GetComponent<Animator>();

				if (animator != nullptr)
				{
					animator->StopPlayAnimation();
				}
				return;
			}

			if (InvenPos.x - (InvenScale.x * 0.5f) <= ItemPos.x && InvenPos.x + (InvenScale.x * 0.5f) >= ItemPos.x
				&& InvenPos.y - (InvenScale.y * 0.5f) <= ItemPos.y && InvenPos.y + (InvenScale.y * 0.5f) >= ItemPos.y)
			{
				mbDrop = false;
				Input::GetInstance()->SetPickItem(this);

				Animator* animator = GetComponent<Animator>();

				if (animator != nullptr)
				{
					animator->StopPlayAnimation();
				}
				return;
			}

			GameObject* player = WorldManager::GetInstance()->GetPlayer();
			if (!player)
				return;

			Transform* playerTr = player->GetComponent<Transform>();
			Vector3 playerPos = playerTr->GetPosition();
			
			ItemPos = playerPos;
			ItemTr->SetPosition(ItemPos);

			Vector3 scale = this->GetWorldSize();
			ItemTr->SetSize(scale);

			mbPick = false;
			Input::GetInstance()->SetPickItem(nullptr);

			if(GetInventory())
				GetInventory()->DeleteItem(this);
			if(GetSlotInventory())
				GetSlotInventory()->DeleteItem(this);

			this->SetInventory(nullptr);
			this->SetSlotInventory(nullptr);

			Animator* animator = GetComponent<Animator>();

			if (animator != nullptr)
			{
				GetComponent<AudioSource>()->Play();
				animator->Play(L"WorldDrop", false);
			}

			SetStage(true);
		}
	}

	if (Input::GetInstance()->GetKeyDown(eKeyCode::RBTN)
		&& mbStage
		&& Input::GetInstance()->GetPickItem() == nullptr
		&& ItemManager::GetInstance()->GetPickUpItem() == false)
	{
		InventoryPanel* invenPanel = UIManager::GetInstance()->GetUiInstance<InventoryPanel>(L"mainInventory");
		InventoryButton* InvenArr = invenPanel->GetInventoryArr();

		bool pickUp = InvenArr->PickUpItem(this);
		if (pickUp)
		{
			this->SetInventory(InvenArr);
			mbStage = false;

			Transform* tr = this->GetComponent<Transform>();
			tr->SetSize(this->GetInvenSize());

			this->GetComponent<Animator>()->StopPlayAnimation();

			ItemManager::GetInstance()->SetPickUpItem(true);
			
			bool render = invenPanel->GetComponent<BaseRenderer>()->GetRenderStop();
			this->GetComponent<BaseRenderer>()->SetRenderStop(render);
		}
		else
		{
			Animator* animator = GetComponent<Animator>();

			if (animator != nullptr)
			{
				
				animator->Play(L"WorldDrop", false);
			}

			GameObject* player = WorldManager::GetInstance()->GetPlayer();
			if (!player)
				return;

			Transform* ItemTr = this->GetComponent<Transform>();
			Vector3 ItemPos = ItemTr->GetPosition();
			Vector3 ItemScale = ItemTr->GetScale();

			Transform* playerTr = player->GetComponent<Transform>();
			Vector3 playerPos = playerTr->GetPosition();

			ItemPos = playerPos;
			ItemTr->SetPosition(ItemPos);

			Vector3 scale = this->GetWorldSize();
			ItemTr->SetSize(scale);
			mbStage = true;
		}
	}

	Input::GetInstance()->SetMouseItemPick(mbPick);
}

void ItemBase::FixedUpdate()
{
	GameObject::FixedUpdate();
}

void ItemBase::Render()
{
	Renderer::ItemDataCB info = {};
	Renderer::ItemDataCB* clear = nullptr;

	// ���� �ȵ� �������̸� ��Ȳ�� �´� ���� �����
	info.pick = mbPick;
	// ���� �κ��丮 �������� �����ε� �Ǻ��� ���ǿ��´�
	// �ִϸ��̼����
	info.stage = mbStage;

	if (mInventory) // ������ �ִ»�Ȳ (�ʷ�)
		info.drop = mInventory->GetDrop();
	else			// ������ ��Ȳ (����)
		info.drop = false;
	

	Vector4 color = Vector4::One;
	if (info.pick)
	{
		color.w = 0.5f;
		
		if (info.drop)
		{
			color *= Vector4(0.2f, 1.0f, 0.2f, 0.5f);
		}
		else
		{
			color *= Vector4(1.0f, 0.2f, 0.2f, 0.5f);
		}
	}
	info.canversColor = color;

	// ��ŷ�� �������� �κ��丮�迭 ���ΰ� �ƴ� �ٸ������ִٸ�
	// ĵ������ ��������ʰ� �þ߸� ���� ���İ��� ���ϰ� ���
	info.OnInventory = mbOnInvnetory;

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::ItemData];
	cb->SetData(&info);
	cb->Bind(eShaderStage::ALL);

	GameObject::Render();
}

void ItemBase::Drop()
{
	GetComponent<Animator>()->Play(L"WorldDrop", false);
}

