#pragma once
#include "CGameObject.h"
#include "CMeshRenderer.h"

using namespace std;
using namespace Math;
using namespace graphics;

class UiBase : public GameObject
{
public:
	struct Event
	{
		void operator=(std::function<void()> func)
		{
			mEvent = std::move(func);
		}

		void operator()()
		{
			if (mEvent)
				mEvent();
		}

		std::function<void()> mEvent;
	};

	UiBase(eUIType type);
	virtual ~UiBase();

	// UI �� �ε� �Ǿ����� �Ҹ��� �ʱ�ȭ �Լ�
	virtual void Initalize();

	// UI �� Ȱ��ȭ�Ǹ� �Ҹ��� �Լ�
	void Active();

	// UI �� ��Ȱ��ȭ�Ǹ� �Ҹ��� �Լ�
	void InActive();

	// UI�� ������Ʈ �ɶ����� ȣ��Ǵ� �Լ�
	virtual void Update();

	// UI ������ �� ������ ������Ʈ
	virtual void FixedUpdate();

	// UI ����
	virtual void Render();

	// �ִϸ��̼� Ȥ�� �ؽ��� ����
	virtual void InitAnimation() {};
	virtual void InitRenderer(const wstring& materialName, const wstring& textureName, const std::wstring& textruepath) {};

	void Render(HDC hdc);

	// UI�� ������� ȣ��Ǵ� �Լ�
	void UIClear();

	std::shared_ptr<Texture2D> ImageLoad(const std::wstring& key, const std::wstring& path);
	void SetChild(UiBase* child);
	void DeleteChild(UiBase* child);

	eUIType GetType() { return mType; }
	void SetType(eUIType type) { mType = type; }
	bool GetIsFullScreen() { return mbFullScreen; }
	void SetIsFullScreen(bool enable) { mbFullScreen = enable; }
	void SetParent(UiBase* parent) { mParent = parent; }

	bool GetIsAble() { return mbEnable; }

protected:
	UiBase* mParent;
	Texture2D* mImage;

protected:
	virtual void OnInit() {};
	virtual void OnActive() {};
	virtual void OnInActive() {};
	virtual void OnUpdate() {};
	virtual void OnRender(HDC hdc) {};
	virtual void OnClear() {};

protected:
	std::vector<UiBase*> mChilds;

	eUIType mType;
	bool mbFullScreen;
	bool mbEnable;
};

