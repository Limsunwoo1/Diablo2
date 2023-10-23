#include "CSelectButtonSystem.h"
#include "FileManager.h"
#include "CResourceManager.h"

#include "CPlayerSelectButton.h"
#include "CSpriteRenderer.h"
#include "CMeshRenderer.h"
#include "CSelectSystemButton.h"
#include "CSceneManager.h"
#include "CWorldManager.h"
#include "CItemManager.h"

#include "CUIManager.h"
#include "Cplayer.h"
#include "CPlayerScript.h"
#include "CCollider2D.h"

#include "CItemBase.h"
#include "CInventoryPanel.h"
#include "CMainPanel.h"
#include "CInventoryButton.h"
#include "CEquipmentButton.h"
#include "CPotionButton.h"

#include "CShoesItem.h"
#include "CBeltItem.h"
#include "CCapItem.h"
#include "CSuitItem.h"
#include "CGloveItem.h"
#include "CWeponItem.h"
#include "CHpPotionItem.h"
#include "CMpPotionItem.h"

#include "CLoaddingScene.h"

#include "CAudioListner.h"
#include "CAudioClip.h"
#include "CAudioSource.h"


SelectButtonSystem::SelectButtonSystem()
	: UiBase(eUIType::Panel)
	, mClickButtonIdx(-1)
	, mClickButton(nullptr)
	, mCreateButton(nullptr)
	, mReturnSceneButton(nullptr)
	, mSelect_Ok_Button(nullptr)
{
}

SelectButtonSystem::~SelectButtonSystem()
{
	for (GameObject* obj : mButton)
	{
		if (obj == nullptr)
			continue;

		delete obj;
		obj = nullptr;
	}
}

void SelectButtonSystem::Initalize()
{
	{
		MeshRenderer* mr = this->AddComponent<MeshRenderer>();

		std::weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		std::weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"RectMaterial");
		mr->SetMesh(mesh);
		mr->SetMaterial(material);

		mr->SetRenderStop(true);
	}

	FileManager::GetInstance()->ReadFold(L"Resource//Data", eFileType::Char);
	int dataCnt = FileManager::GetInstance()->GetDataSize_Int(eFileType::Char);

	vector<wstring> temp = {};
	for (UINT i = 0; i < dataCnt; ++i)
	{
		temp.emplace_back(FileManager::GetInstance()->GetData(eFileType::Char, i));
	}

	mItemData.resize(temp.size());

	auto strFunc = [this](std::wstring& str, UINT& CurPos) ->std::wstring
		{
			std::wstring data = L"";

			CurPos = str.find_first_of(L",");
			str.erase(0, CurPos + 1);

			CurPos = str.find_first_of(L"\n");
			data = str.substr(0, CurPos);

			return data;
		};

	UINT Index = 0;
	UINT CurPos = 0;
	PlayerSelectButton::PlayerInfo info;
	for (wstring& str : temp)
	{
		std::wstring data = L"";
		// 타입추출
		data = strFunc(str, CurPos);
		int type = stoi(data);
		// \n 을 지우기위해 + 1
		str.erase(0, CurPos + 1);

		// 레벨 추출
		data = strFunc(str, CurPos);
		int level = stoi(data);
		str.erase(0, CurPos + 1);

		// Hp 추출
		data = strFunc(str, CurPos);
		float hp = stoi(data);
		str.erase(0, CurPos + 1);

		// Mp 추출
		data = strFunc(str, CurPos);
		float mp = stoi(data);
		str.erase(0, CurPos + 1);

		// Exp추출
		data = strFunc(str, CurPos);
		float exp = stoi(data);
		str.erase(0, CurPos + 1);

		// Name 추출
		data = strFunc(str, CurPos);

		wstring name = data;
		str.erase(0, CurPos + 1);

		Vector3 pos = Vector3::Zero;
		{ // Position
			vector<float> posData = {};

			for (int i = 0; i < 3; ++i)
			{
				// 데이터 읽기 x y z
				CurPos = str.find_first_of(L",");
				str.erase(0, CurPos + 1);

				CurPos = str.find_first_of(L"\n");
				data = str.substr(0, CurPos);

				posData.push_back(stoi(data));
				str.erase(0, CurPos + 1);
			}

			if (posData.size() != 3)
				continue;

			pos = Vector3(posData[0], posData[1], posData[2]);
		}

		{
			mItemData[Index] = (str);
		}

		info.CharterType = type;
		info.Level = level;
		info.Hp = hp;
		info.Mp = mp;
		info.Exp = exp;
		info.Name = name;
		info.Postion = pos;

		PlayerSelectButton* pushButton = new PlayerSelectButton();
		pushButton->Initalize();
		pushButton->SetInfo(info);
		pushButton->SetDataPathIndex(Index);

		mButton.push_back(pushButton);
		Index++;
	}
	ButtonPositionInit();

	// Button
	mCreateButton = new SelectSystemButton();
	{
		// Init
		mCreateButton->Initalize();
		mCreateButton->BindEvnet(std::bind(&SelectButtonSystem::CreateButton, this));
		mCreateButton->SetName(L"CreateButton");

		AudioSource* source = mCreateButton->AddComponent<AudioSource>();

		std::weak_ptr<AudioClip> clip = ResourceManager::GetInstance()->Load<AudioClip>(L"ButtonClick", L"SoundResource\\Effect\\button.wav");
		source->SetClip(clip);

		//Transform
		Transform* tr = mCreateButton->GetComponent<Transform>();
		tr->SetPosition(Vector3(-4.5f * 100.f, -380.f, 0.0f));
		tr->SetScale(Vector3(3.f, 1.f, 0.0f));

		// Renderer
		SpriteRenderer* sr = mCreateButton->AddComponent<SpriteRenderer>();
		weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"Button1Material");
		sr->SetMesh(mesh);
		sr->SetMaterial(material);

		// Texture Setting
		std::shared_ptr<Texture2D> tex1 = std::make_shared<Texture2D>();
		tex1.get()->Load(L"UI//CreateNewCharterOn.png");

		std::shared_ptr<Texture2D> tex2 = std::make_shared<Texture2D>();
		tex2.get()->Load(L"UI//CreateNewChaterClick.png");

		mCreateButton->SetButtonTex(0, tex1);
		mCreateButton->SetButtonTex(1, tex2);
		ResourceManager::GetInstance()->Insert<Texture2D>(L"CreateCharterOn", tex1);
		ResourceManager::GetInstance()->Insert<Texture2D>(L"CreateCharterClick", tex2);
	}

	mReturnSceneButton = new SelectSystemButton();
	{
		// Init
		mReturnSceneButton->Initalize();
		auto fun = std::bind(&SelectButtonSystem::RetrunSceneButton, this);
		mReturnSceneButton->BindEvnet(fun);
		mReturnSceneButton->SetName(L"ReternButton");

		AudioSource* source = mReturnSceneButton->AddComponent<AudioSource>();

		std::weak_ptr<AudioClip> clip = ResourceManager::GetInstance()->Load<AudioClip>(L"ButtonClick", L"SoundResource\\Effect\\button.wav");
		source->SetClip(clip);

		//Transform
		Transform* tr = mReturnSceneButton->GetComponent<Transform>();
		tr->SetPosition(Vector3(3.5f * 100.f, -380.f, 0.0f));
		tr->SetScale(Vector3(2.f, 1.f, 0.0f));

		// Renderer
		SpriteRenderer* sr = mReturnSceneButton->AddComponent<SpriteRenderer>();
		weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"Button2Material");
		sr->SetMesh(mesh);
		sr->SetMaterial(material);

		// Texture Setting
		std::shared_ptr<Texture2D> tex1 = std::make_shared<Texture2D>();
		tex1.get()->Load(L"UI//CancelButtonOn.png");

		std::shared_ptr<Texture2D> tex2 = std::make_shared<Texture2D>();
		tex2.get()->Load(L"UI//CancelButtonClick.png");

		mReturnSceneButton->SetButtonTex(0, tex1);
		mReturnSceneButton->SetButtonTex(1, tex2);
		ResourceManager::GetInstance()->Insert<Texture2D>(L"CancelButtonOn", tex1);
		ResourceManager::GetInstance()->Insert<Texture2D>(L"CancelButtonClick", tex2);
	}

	mSelect_Ok_Button = new SelectSystemButton();
	{
		// Init
		mSelect_Ok_Button->Initalize();
		auto fun = std::bind(&SelectButtonSystem::Select_Ok_Button, this);
		mSelect_Ok_Button->BindEvnet(fun);
		mSelect_Ok_Button->SetName(L"OkButton");

		AudioSource* source = mSelect_Ok_Button->AddComponent<AudioSource>();

		std::weak_ptr<AudioClip> clip = ResourceManager::GetInstance()->Load<AudioClip>(L"ButtonClick", L"SoundResource\\Effect\\button.wav");
		source->SetClip(clip);

		//Transform
		Transform* tr = mSelect_Ok_Button->GetComponent<Transform>();
		tr->SetPosition(Vector3(6.5f * 100.f, -380.f, 0.0f));
		tr->SetScale(Vector3(2.f, 1.f, 0.0f));

		// Renderer
		SpriteRenderer* sr = mSelect_Ok_Button->AddComponent<SpriteRenderer>();
		weak_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
		weak_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"Button3Material");
		sr->SetMesh(mesh);
		sr->SetMaterial(material);

		// Texture Setting
		std::shared_ptr<Texture2D> tex1 = std::make_shared<Texture2D>();
		tex1.get()->Load(L"UI//OkButtonOn.png");

		std::shared_ptr<Texture2D> tex2 = std::make_shared<Texture2D>();
		tex2.get()->Load(L"UI//OkButtonClick.png");

		mSelect_Ok_Button->SetButtonTex(0, tex1);
		mSelect_Ok_Button->SetButtonTex(1, tex2);
		ResourceManager::GetInstance()->Insert<Texture2D>(L"OkButtonOn", tex1);
		ResourceManager::GetInstance()->Insert<Texture2D>(L"OkButtonClick", tex2);
	}

	this->SetChild(mCreateButton);
	this->SetChild(mReturnSceneButton);
	this->SetChild(mSelect_Ok_Button);
}

void SelectButtonSystem::Update()
{
	UiBase::Update();

	for (PlayerSelectButton* obj : mButton)
	{
		if (obj == nullptr)
			continue;

		obj->Update();
	}
}

void SelectButtonSystem::FixedUpdate()
{
	UiBase::FixedUpdate();

	for (PlayerSelectButton* obj : mButton)
	{
		if (obj == nullptr)
			continue;

		obj->FixedUpdate();
	}
}

void SelectButtonSystem::Render()
{
	UiBase::Render();

	for (PlayerSelectButton* obj : mButton)
	{
		if (obj == nullptr)
			continue;

		obj->Render();
	}
}

void SelectButtonSystem::CreateButton()
{
	/*int count = FileManager::GetInstance()->GetDataSize_Int(eFileType::Char);

	bool seucess = FileManager::GetInstance()->
		CreateSaveFile(L"..//Resource//Data",
			L"test" + std::to_wstring(count + 1),
			eCharType::Sorceress);

	if (!seucess)
		return;

	wstring str = FileManager::GetInstance()->GetCreateData();
	if (str == L"")
		return;*/

	mCreateButton->GetComponent<AudioSource>()->Play();
	CreateSelectButton();
}

void SelectButtonSystem::RemoveButton()
{
}

void SelectButtonSystem::RetrunSceneButton()
{
	mReturnSceneButton->GetComponent<AudioSource>()->Play();
	SceneManager::GetInstance()->LoadScene(eSceneType::MainTitle);
	mClickButton = nullptr;
}

void SelectButtonSystem::Select_Ok_Button()
{
	if (mClickButton == nullptr)
		return;

	mSelect_Ok_Button->GetComponent<AudioSource>()->Play();

	PlayerSelectButton::PlayerInfo info = mClickButton->GetInfo();

	Player* player = new Player();
	player->SetLayerType(eLayerType::Player);
	player->Initalize();
	player->SetSavePathIndex(mClickButton->GetDataPathIndex());

	{
		player->SetCharType((eCharType)info.CharterType);
		player->SetLevel(info.Level);
		player->SetHP(info.Hp);
		player->SetMP(info.Mp);
		player->SetExp(info.Exp);
		player->SetSaveName(string(info.Name.begin(), info.Name.end()));
	}

	PlayerScript* sc = player->AddComponent<PlayerScript>();

	Collider2D* collider = player->AddComponent<Collider2D>();
	collider->SetSize(Vector2(0.25f, 0.25f));
	collider->SetType(eColliderType::Rect);

	Transform* tr = player->GetComponent<Transform>();
	tr->SetPosition(Vector3(info.Postion.x, info.Postion.y, info.Postion.z));
	tr->SetSize(Vector3(300.f, 300.f, 1.f));


	player->InitAnimation();
	player->PlayAnimation(L"Idle0");

	WorldManager::GetInstance()->SetPlayer(player);

	{
		Scene* scene = SceneManager::GetInstance()->GetScene(eSceneType::Loadding);
		LoaddingScene* load = dynamic_cast<LoaddingScene*>(scene);
		if (load == nullptr)
			return;

		load->SeteSceneType(eSceneType::Play);

		SceneManager::GetInstance()->LoadScene(eSceneType::Loadding);
	}

	if (mClickButton->GetDataPathIndex() >= mItemData.size())
		return;

	if (mItemData[mClickButton->GetDataPathIndex()] == L"")
		return;

	InventorySetting(mClickButton->GetDataPathIndex());
}

void SelectButtonSystem::ButtonPositionInit()
{
	// 위치값 조정해서 자동으로위치 조정 해야함
	float leftX = -530.f;
	float leftY = 220.f;

	float rightX = 1 * 100;

	float sizeX = 350.f;
	float sizeY = 180.f;

	float offset = 6.2f * 100;

	int cntX = 0;
	int cntY = 0;


	for (PlayerSelectButton* obj : mButton)
	{
		if (obj == nullptr)
			continue;

		obj->SetSystem(this);

		// 왼쪽라인 세이브 파일
		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(Vector3(leftX, leftY - (sizeY * cntX), 0.0f));
		tr->SetSize(Vector3(sizeX, sizeY, 1.0f));


		if (cntX >= 3)
		{
			// 오른쪽 라인 세이브 파일
			tr->SetPosition(Vector3(leftX + offset, leftY - (sizeY * cntY), 0.0f));
			tr->SetSize(Vector3(sizeX, sizeY, 1.0f));

			cntY++;
		}

		cntX++;
	}
}

void SelectButtonSystem::CreateSelectButton()
{
	int index = FileManager::GetInstance()->GetDataSize_Int(eFileType::Char);
	bool seucess = FileManager::GetInstance()->
		CreateSaveFile(L"..//Resource//Data",
			L"test" + to_wstring(index),
			eCharType::Sorceress);

	if (!seucess)
		return;

	wstring str = FileManager::GetInstance()->GetCreateData();

	if (str == L"")
		return;


	PlayerSelectButton::PlayerInfo info;
	UINT CurPos = 0;
	std::wstring data = L"";
	// 타입추출
	{
		CurPos = str.find_first_of(L",");
		str.erase(0, CurPos + 1);

		CurPos = str.find_first_of(L"\n");
		data = str.substr(0, CurPos);
	}
	int type = stoi(data);
	// \n 을 지우기위해 + 1
	str.erase(0, CurPos + 1);

	// 레벨 추출
	{
		CurPos = str.find_first_of(L",");
		str.erase(0, CurPos + 1);

		CurPos = str.find_first_of(L"\n");
		data = str.substr(0, CurPos);
	}
	int level = stoi(data);
	str.erase(0, CurPos + 1);

	// Hp 추출
	{
		CurPos = str.find_first_of(L",");
		str.erase(0, CurPos + 1);

		CurPos = str.find_first_of(L"\n");
		data = str.substr(0, CurPos);
	}
	float hp = stoi(data);
	str.erase(0, CurPos + 1);

	// Mp 추출
	{

		CurPos = str.find_first_of(L",");
		str.erase(0, CurPos + 1);

		CurPos = str.find_first_of(L"\n");
		data = str.substr(0, CurPos);
	}
	float mp = stoi(data);
	str.erase(0, CurPos + 1);

	// Exp추출
	{
		CurPos = str.find_first_of(L",");
		str.erase(0, CurPos + 1);

		CurPos = str.find_first_of(L"\n");
		data = str.substr(0, CurPos);
	}
	float exp = stoi(data);
	str.erase(0, CurPos + 1);

	// Name 추출
	{
		CurPos = str.find_first_of(L",");
		str.erase(0, CurPos + 1);

		CurPos = str.find_first_of(L"\n");
		data = str.substr(0, CurPos);
	}
	wstring name = data;
	str.erase(0, CurPos + 1);

	Vector3 pos = Vector3::Zero;
	{ // Position
		vector<float> posData = {};

		//{
		//	CurPos = str.find_first_of(L",");
		//	str.erase(0, CurPos + 1);

		//	CurPos = str.find_first_of(L"\n");
		//	data = str.substr(0, CurPos);
		//}
		//posData.push_back(stoi(data));
		//str.erase(0, CurPos + 1);

		for (int i = 0; i < 3; ++i)
		{
			// 데이터 읽기 x y z
			CurPos = str.find_first_of(L",");
			str.erase(0, CurPos + 1);

			CurPos = str.find_first_of(L"\n");
			data = str.substr(0, CurPos);

			posData.push_back(stoi(data));
			str.erase(0, CurPos + 1);
		}

		if (posData.size() != 3)
			return;

		pos = Vector3(posData[0], posData[1], posData[2]);
	}

	info.CharterType = type;
	info.Level = level;
	info.Hp = hp;
	info.Mp = mp;
	info.Exp = exp;
	info.Name = name;
	info.Postion = pos;

	PlayerSelectButton* pushButton = new PlayerSelectButton();
	pushButton->Initalize();
	pushButton->SetInfo(info);
	pushButton->SetDataPathIndex(index);

	mButton.emplace_back(pushButton);

	ButtonPositionInit();
}

void SelectButtonSystem::InventorySetting(UINT index)
{
	UINT CurPos = 0;
	std::wstring data = L"";
	while (!mItemData[index].empty())
	{
		// 타입추출
		{
			CurPos = mItemData[index].find_first_of(L",");
			mItemData[index].erase(0, CurPos + 1);

			CurPos = mItemData[index].find_first_of(L"\n");
			data = mItemData[index].substr(0, CurPos);
		}

		if (data == L"")
			return;

		int type = stoi(data);
		// \n 을 지우기위해 + 1
		mItemData[index].erase(0, CurPos + 1);

		// 버튼추출
		{
			CurPos = mItemData[index].find_first_of(L",");
			mItemData[index].erase(0, CurPos + 1);

			CurPos = mItemData[index].find_first_of(L"\n");
			data = mItemData[index].substr(0, CurPos);
		}
		int stage = stoi(data);
		// \n 을 지우기위해 + 1
		mItemData[index].erase(0, CurPos + 1);

		// X인덱스추출
		{
			CurPos = mItemData[index].find_first_of(L",");
			mItemData[index].erase(0, CurPos + 1);

			CurPos = mItemData[index].find_first_of(L"\n");
			data = mItemData[index].substr(0, CurPos);
		}
		int X = stoi(data);
		// \n 을 지우기위해 + 1
		mItemData[index].erase(0, CurPos + 1);

		// Y인덱스추출
		{
			CurPos = mItemData[index].find_first_of(L",");
			mItemData[index].erase(0, CurPos + 1);

			CurPos = mItemData[index].find_first_of(L"\n");
			data = mItemData[index].substr(0, CurPos);
		}
		int Y = stoi(data);
		// \n 을 지우기위해 + 1
		mItemData[index].erase(0, CurPos + 1);

		Dynamic_ItemSetting(type, stage, X, Y);
		CurPos = 0;
	}
}

void SelectButtonSystem::Dynamic_ItemSetting(UINT type, UINT stage, UINT x, UINT y)
{
	ItemBase* item = nullptr;
	switch ((eEquipmentType)type)
	{
	case eEquipmentType::Shoes: item = new ShoesItem(L"Shoes"); break;
	case eEquipmentType::Glove: item = new GloveItem(L"Glove"); break;
	case eEquipmentType::Belt:	item = new BeltItem(L"Belt");	break;
	case eEquipmentType::Wepon: item = new WeponItem(L"Wepon");	break;
	case eEquipmentType::Suit:	item = new SuitItem(L"Suit");	break;
	case eEquipmentType::Cap:	item = new CapItem(L"Cap");		break;
	case eEquipmentType::HpPotion:item = new HpPotionItem();	break;
	case eEquipmentType::MpPotion:item = new MpPotionItem();	break;
	default: break;
	}

	if (item == nullptr)
		return;

	ItemManager::GetInstance()->PushItem(item);

	item->Initalize();
	item->SetIndex(x, y);

	if (stage == 0)
	{
		InventoryPanel* invenPanel = UIManager::GetInstance()->GetUiInstance<InventoryPanel>(L"mainInventory");
		InventoryButton* invenButton = nullptr;

		if (invenPanel != nullptr)
		{
			invenButton = invenPanel->GetInventoryArr();
		}

		if (invenButton != nullptr)
		{
			invenButton->SetDrop(true);

			invenButton->SetIndex(x, y);
			invenButton->DropItem(item);
		}
	}
	else
	{
		SlotItemSetting(item);
	}
}

void SelectButtonSystem::SlotItemSetting(ItemBase* item)
{
	InventoryPanel* invenPanel = UIManager::GetInstance()->GetUiInstance<InventoryPanel>(L"mainInventory");
	MainPanel* mainPanel = UIManager::GetInstance()->GetUiInstance<MainPanel>(L"mainPanel");

	if (!mainPanel) return;
	if (!invenPanel) return;

	vector<EquipmentButton*> InvenPanelSlot = invenPanel->GetSlotButton();
	vector<EquipmentButton*> MainPanelSlot = mainPanel->GetPotionSlot();

	eEquipmentType type = item->GetItemType();

	if (type != eEquipmentType::HpPotion && type != eEquipmentType::MpPotion)
	{
		for (EquipmentButton* button : InvenPanelSlot)
		{
			if (type == button->GetEquipmentType()
				&& button->GetItem() == nullptr)
			{
				button->DropItem(item);
			}
		}
	}
	else
	{
		for (EquipmentButton* button : InvenPanelSlot)
		{
			if (!button->GetUsed())
			{
				button->DropItem(item);

			}
		}
	}
}
