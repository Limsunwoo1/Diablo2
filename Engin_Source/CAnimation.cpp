#include "CAnimation.h"
#include "CTime.h"
#include "CRenderer.h"


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

UINT Animation::Update()
{
	if (mbComplete)
		return FAIL_EVENT;

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

		return mIndex;
	}

	return FAIL_EVENT;
}

void Animation::FixedUpdate()
{

}

void Animation::Render()
{

}

void Animation::Create(const wstring& name, shared_ptr<Texture2D> atlas
	, Vector2 leftTop, Vector2 size, Vector2 offset
	, UINT spriteLenght, float duration)
{
	mAnimationName = name;

	mAtlas = atlas;
	float width = (float)atlas->GetWidth();
	float height = (float)atlas->GetHeight();

	for (UINT i = 0; i < spriteLenght; ++i)
	{
		// API �ʹ� �ٸ��� 0~ 1 ������ ������ǥ�� ��ġ�� ǥ���Ѵ�
		Sprite sprite = {};
		sprite.leftTop = Vector2((leftTop.x + (size.x * (float)i)) / width
								, leftTop.y / height);
		sprite.size = Vector2(size.x / width, size.y / height);
		sprite.offset = offset;
		sprite.duration = duration;
		sprite.atlasSize = Vector2(200.0f / width, 200.0f / height);

		mSpriteSheet.push_back(sprite);
	}
}

void Animation::Create(const wstring& name, shared_ptr<Texture2D> atlas, Vector2 leftTop, float size, Vector2 offset, UINT spriteLenght, float duration)
{
	mAnimationName = name;

	mAtlas = atlas;
	float width = (float)atlas->GetWidth();
	float height = (float)atlas->GetHeight();

	for (int i = 0; i < spriteLenght; ++i)
	{
		// API �ʹ� �ٸ��� 0~ 1 ������ ������ǥ�� ��ġ�� ǥ���Ѵ�
		Sprite sprite = {};
		sprite.leftTop = Vector2(0.0f,  (size * (float)i) / height);
		sprite.size = Vector2(1.0f, size / height);
		sprite.offset = offset;
		sprite.duration = duration;
		sprite.atlasSize = Vector2(1.0f, 1.0f);

		mSpriteSheet.push_back(sprite);
	}
}

void Animation::BindShader()
{
	mAtlas->BindShader(eShaderStage::PS, 12);

	ConstantBuffer* CB = Renderer::constantBuffers[(UINT)eCBType::Animation];

	Renderer::AnimationCB info = {};
	info.type = (UINT)eAnimationType::SecondDimension;
	info.leftTop = mSpriteSheet[mIndex].leftTop;
	info.offset = mSpriteSheet[mIndex].offset;
	info.size = mSpriteSheet[mIndex].size;
	info.atlasSize = mSpriteSheet[mIndex].atlasSize;

	CB->Bind(&info);
	CB->SetPipline(eShaderStage::PS);
}

void Animation::Reset()
{
	mTime = 0.0f;
	mIndex = 0;
	mbComplete = false;
}

void Animation::Clear()
{
	mAtlas->Clear(12);

	ConstantBuffer* cb = Renderer::constantBuffers[(UINT)eCBType::Animation];
	Renderer::AnimationCB info = {};
	info.type = (UINT)eCBType::Animation;

	cb->Bind(&info);
	cb->SetPipline(eShaderStage::PS);
}