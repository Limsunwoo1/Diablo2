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

	// UI 가 로드 되었을때 불리는 초기화 함수
	virtual void Initalize();

	// UI 가 활성화되면 불리는 함수
	void Active();

	// UI 가 비활성화되면 불리는 함수
	void InActive();

	// UI가 업데이트 될때마다 호출되는 함수
	virtual void Update();

	// UI 렌더링 전 마지막 업데이트
	virtual void FixedUpdate();

	// UI 렌더
	virtual void Render();

	// 애니메이션 혹은 텍스쳐 세팅
	virtual void InitAnimation() {};
	virtual void InitRenderer(const wstring& materialName, const wstring& textureName, const std::wstring& textruepath) {};

	void Render(HDC hdc);

	// UI가 사라질때 호출되는 함수
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

