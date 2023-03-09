#include "CAnimator.h"

Animator::Animator()
	: Component(eComponentType::Animator)
	, mAnimations{}
	, mEvents{}
	, mActiveAnimation(nullptr)
	, mbLoop(false)
{
	
}

Animator::~Animator()
{
	for (auto anim : mAnimations)
	{
		delete anim.second;
		anim.second = nullptr;
	}

	for (auto evt : mEvents)
	{
		delete evt.second;
		evt.second = nullptr;
	}
}

void Animator::Initalize()
{

}

void Animator::Update()
{
	if (mActiveAnimation == nullptr)
		return;

	if (mActiveAnimation->IsComplete() && mbLoop)
	{
		Events* events = FindEvents(mActiveAnimation->AnimationName());

		if (events)
			events->mCompleteEvent();

		mActiveAnimation->Update();
	}
}

void Animator::FixedUpdate()
{

}

void Animator::Render()
{

}


bool Animator::Create(const wstring& name, shared_ptr<Texture2D> atlas, Vector2 leftTop
	, Vector2 size, Vector2 offset, UINT columnLength
	, UINT spriteLength, float duation)
{
	if (atlas == nullptr)
		return false;

	Animation* animation = Animator::FindAnimation(name);
	if (animation != nullptr)
		return false;

	animation = new Animation();
	animation->Create(name, atlas, leftTop, size, offset, columnLength, spriteLength, duation);

	mAnimations.insert(make_pair(name, animation));
}

Animation* Animator::FindAnimation(const wstring& name)
{
	map<wstring, Animation*>::iterator iter;
	iter = mAnimations.find(name);

	if (iter == mAnimations.end())
		return nullptr;

	return iter->second;
}

Animator::Events* Animator::FindEvents(const wstring& name)
{
	map<wstring, Events*>::iterator iter;
	iter = mEvents.find(name);

	if (iter == mEvents.end())
		return nullptr;

	return iter->second;
}

void Animator::Play(const wstring& name, bool loop)
{
	Animation* preveAnimation = mActiveAnimation;
	Events* events = FindEvents(preveAnimation->AnimationName());

	if (events)
		events->mEndEvent();

	mActiveAnimation = FindAnimation(name);
	if (mActiveAnimation)
		mActiveAnimation->Reset();

	mbLoop = loop;

	events = FindEvents(mActiveAnimation->AnimationName());

	if (events)
		events->mStarteEvent();
}

void Animator::Binds()
{
	if (mActiveAnimation == nullptr)
		return;

	mActiveAnimation->BindShader();
}

void Animator::Clear()
{
	if (mActiveAnimation == nullptr)
		return;

	mActiveAnimation->Clear();
}

function<void()>& Animator::GetStartEvent(const wstring& name)
{
	Events* events = FindEvents(name);

	return events->mStarteEvent.mEvent;
}

function<void()>& Animator::GetCompleteEvent(const wstring& name)
{
	Events* events = FindEvents(name);

	return events->mCompleteEvent.mEvent;
}

function<void()>& Animator::GetEndEvent(const wstring& name)
{
	Events* events = FindEvents(name);

	return events->mEndEvent.mEvent;
}
