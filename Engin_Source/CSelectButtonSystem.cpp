#include "CSelectButtonSystem.h"
#include "FileManager.h"
#include "CResourceManager.h"

#include "CPlayerSelectButton.h"
#include "CSpriteRenderer.h"
#include "CMeshRenderer.h"


SelectButtonSystem::SelectButtonSystem()
	: UiBase(eUIType::Panel)
	, mClickButtonIdx(-1)
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

	for (wstring& str : temp)
	{
	}
	mButton.emplace_back(new PlayerSelectButton());

	int cnt = 0;

	float left = -0.68;
	float right = 0.7;

	float yScale = 0.44;
	float xScale = yScale * 2;
	
	for (PlayerSelectButton* obj : mButton)
	{
		if (obj == nullptr)
			continue;

		obj->Initalize();

		Transform* tr = obj->GetComponent<Transform>();
		tr->SetPosition(Vector3(-3.8, 4.2, 0.0f));
		tr->SetScale(Vector3(4, 2, 0.0f));
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
