#include "CSelectButtonSystem.h"
#include "FileManager.h"
#include "CResourceManager.h"

#include "CPlayerSelectButton.h"
#include "CSpriteRenderer.h"
#include "CMeshRenderer.h"


SelectButtonSystem::SelectButtonSystem()
	: UiBase(eUIType::Panel)
	, mClickButtonIdx(-1)
	, mClickButton(nullptr)
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
	}

	FileManager::GetInstance()->ReadFold(L"Resource//Data", eFileType::Char);
	int dataCnt = FileManager::GetInstance()->GetDataSize_Int(eFileType::Char);

	vector<wstring> temp = {};
	for (UINT i = 0; i < dataCnt; ++i)
	{
		temp.emplace_back(FileManager::GetInstance()->GetData(eFileType::Char, i));
	}

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

			// Tag 먼저 지움
			CurPos = str.find_first_of(L",");
			data = str.substr(0, CurPos);
			str.erase(0, CurPos + 1);

			for (int i = 0; i < 3; ++i)
			{
				// 데이터 읽기 x y z
				CurPos = str.find_first_of(L",");
				data = str.substr(0, CurPos);
				str.erase(0, CurPos + 1);

				posData.emplace_back(stoi(data));
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

		mButton.emplace_back(pushButton);
	}
	
	// 위치값 조정해서 자동으로위치 조정 해야함
	int a =
	/*float leftX = -3.8f;
	float leftY = 4.2f;

	float rightX = 1;

	float sizeX = 4.f;
	float sizeY = 2.f;

	int cnt = 0;
	for (PlayerSelectButton* obj : mButton)
	{
		if (obj == nullptr)
			continue;

		int num = cnt % 2;

		obj->Initalize();
		obj->SetSystem(this);

		if (num == 0)
		{
			int y = cnt / 2;

			Transform* tr = obj->GetComponent<Transform>();
			tr->SetPosition(Vector3(leftX , leftY + (sizeY * y), 0.0f));
			tr->SetScale(Vector3(sizeX, sizeY, 0.0f));
		}
		else
		{
			int x = cnt / 2;

			Transform* tr = obj->GetComponent<Transform>();
			tr->SetPosition(Vector3(, leftY, 0.0f));
			tr->SetScale(Vector3(sizeX, sizeY, 0.0f));
		}*/

		cnt++;
	}
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
	for (PlayerSelectButton* obj : mButton)
	{
		if (obj == nullptr)
			continue;

		obj->Render();
	}
}
