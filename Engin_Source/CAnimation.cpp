#include "CAnimation.h"
#include "CTime.h"


Animation::Animation()
	: mAnimator(nullptr)
	, mAtlas(nullptr)
	, mSpriteSheet{}
	, mIndex(-1) // 0 ��°�� �ε����ΰ�찡�����Ƿ� ������ �ʱ�ȭ
	, mTime(0.0f)
	, mbComplete(false)
{

}

Animation::~Animation()
{

}

void Animation::Update()
{
	if (mbComplete)
		return;

	// �ð�üũ
	mTime += Time::GetInstance()->DeltaTime();

	// ���� �ð��� �ش� �������� �����ð��� �Ѿ��
	// �������������� �̵�
	if (mSpriteSheet[mIndex].duration < mTime)
	{
		mTime -= mSpriteSheet[mIndex].duration;
		++mIndex;

		if (mSpriteSheet.size() <= mIndex)
		{
			mbComplete = true;
			mIndex = (int)mSpriteSheet.size() - 1;
		}
	}
}

void Animation::FixedUpdate()
{

}

void Animation::Render()
{

}

void Animation::Create(const wstring& name, shared_ptr<Texture2D> atlas
	, Vector2 leftTop, Vector2 size, Vector2 offset
	, UINT columnLength, UINT spriteLenght, float duration)
{

}

void Animation::BindShader()
{

}

void Animation::Reset()
{
	mTime = 0.0f;
	mIndex = 0;
	mbComplete = false;
}

void Animation::Clear()
{

}