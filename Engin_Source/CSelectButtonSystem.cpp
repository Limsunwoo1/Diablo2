#include "CSelectButtonSystem.h"
#include "FileManager.h"
#include "CResourceManager.h"

#include "CPlayerSelectButton.h"
#include "CSpriteRenderer.h"
#include "CMeshRenderer.h"
#include "CSelectSystemButton.h"
#include "CSceneManager.h"
#include "CWorldManager.h"

#include "Cplayer.h"
#include "CPlayerScript.h"
#include "CCollider2D.h"


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
	for (PlayerSelectButton* obj : mButton)
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

	UINT Index = 0;
	UINT CurPos = 0;
	PlayerSelectButton::PlayerInfo info;
	for (wstring& str : temp)
	{
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
				continue;

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
		pushButton->SetDataPathIndex(Index);

		mButton.emplace_back(pushButton);
		Index++;
	}
	ButtonPositionInit();

	// Button
	mCreateButton = new SelectSystemButton();
	{
		// Init
		mCreateButton->Initalize();
		auto fun = std::bind(&SelectButtonSystem::CreateButton, this);
		mCreateButton->BindEvnet(fun);

		//Transform
		Transform* tr = mCreateButton->GetComponent<Transform>();
		tr->SetPosition(Vector3(-4.5f, -2.9f, 0.0f));
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

		//Transform
		Transform* tr = mReturnSceneButton->GetComponent<Transform>();
		tr->SetPosition(Vector3(3.5f, -2.9f, 0.0f));
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

		//Transform
		Transform* tr = mSelect_Ok_Button->GetComponent<Transform>();
		tr->SetPosition(Vector3(6.5f, -2.9f, 0.0f));
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

	CreateSelectButton();
}

void SelectButtonSystem::RemoveButton()
{
}

void SelectButtonSystem::RetrunSceneButton()
{
	SceneManager::GetInstance()->LoadScene(eSceneType::MainTitle);
	mClickButton = nullptr;
}

void SelectButtonSystem::Select_Ok_Button()
{
	if (mClickButton == nullptr)
		return;

	PlayerSelectButton::PlayerInfo info = mClickButton->GetInfo();

	Player* player = new Player();
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
	collider->SetSize(Vector2(0.5f, 0.5f));
	collider->SetType(eColliderType::Rect);

	Transform* tr = player->GetComponent<Transform>();
	tr->SetPosition(Vector3(info.Postion.x, info.Postion.y, info.Postion.z));
	tr->SetScale(Vector3(3.0f, 3.0f, 1.0f));

	SpriteRenderer* spr = player->AddComponent<SpriteRenderer>();
	std::shared_ptr<Mesh> mesh = ResourceManager::GetInstance()->Find<Mesh>(L"RectMesh");
	std::shared_ptr<Material> material = ResourceManager::GetInstance()->Find<Material>(L"SpriteMaterial");

	spr->SetMesh(mesh);
	spr->SetMaterial(material);

	player->InitAnimation();
	player->PlayAnimation(L"Idle0");

	WorldManager::GetInstance()->SetPlayer(player);
	SceneManager::GetInstance()->LoadScene(eSceneType::Play);
}

void SelectButtonSystem::ButtonPositionInit()
{
	// 위치값 조정해서 자동으로위치 조정 해야함
	float leftX = -3.8f;
	float leftY = 4.2f;

	float rightX = 1;

	float sizeX = 4.f;
	float sizeY = 2.f;

	float offset = 6.2f;

	int cntX = 0;
	int cntY = 0;


	for (PlayerSelectButton* obj : mButton)
	{
		if (obj == nullptr)
			continue;

		obj->Initalize();
		obj->SetSystem(this);

		// 왼쪽라인 세이브 파일
		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(Vector3(leftX, leftY - (sizeY * cntX), 0.0f));
		tr->SetScale(Vector3(sizeX, sizeY, 0.0f));


		if (cntX >= 3)
		{
			// 오른쪽 라인 세이브 파일
			tr->SetPosition(Vector3(leftX + offset, leftY - (sizeY * cntY), 0.0f));
			tr->SetScale(Vector3(sizeX, sizeY, 0.0f));

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
