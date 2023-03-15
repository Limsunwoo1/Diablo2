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

	Events* events = FindEvents(mActiveAnimation->AnimationName());

	if (mActiveAnimation->IsComplete())
	{
		if (events)
			events->mCompleteEvent();

		if(mbLoop)
		mActiveAnimation->Reset();
	}

	UINT spriteIndex = mActiveAnimation->Update();

	if ((spriteIndex != FAIL_EVENT) && events != nullptr)
	{
		if(events->mEvents[spriteIndex].mEvent)
			events->mEvents[spriteIndex].mEvent();
	}
}

void Animator::FixedUpdate()
{

}

void Animator::Render()
{

}


bool Animator::Create(const wstring& name, shared_ptr<Texture2D> atlas, Vector2 leftTop
	, Vector2 size, Vector2 offset
	, UINT spriteLength, float duation)
{
	if (atlas == nullptr)
		return false;

	Animation* animation = Animator::FindAnimation(name);
	if (animation != nullptr)
		return false;

	animation = new Animation();
	animation->Create(name, atlas, leftTop, size, offset, spriteLength, duation);

	mAnimations.insert(make_pair(name, animation));

	Events* events = new Events();
	events->mEvents.resize(spriteLength);
	mEvents.insert(make_pair(name, events));
}

bool Animator::Create(const wstring& name, shared_ptr<Texture2D> atlas, Vector2 leftTop, float size, Vector2 offset, UINT spriteLength, float duation)
{
	if (atlas == nullptr)
		return false;

	Animation* animation = Animator::FindAnimation(name);
	if (animation != nullptr)
		return false;

	animation = new Animation();
	animation->Create(name, atlas, leftTop, size, offset, spriteLength, duation);

	mAnimations.insert(make_pair(name, animation));

	Events* events = new Events();
	events->mEvents.resize(spriteLength);
	mEvents.insert(make_pair(name, events));
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
	Events* events = nullptr;
	if(preveAnimation)
		events = FindEvents(preveAnimation->AnimationName());

	if (events)
		events->mEndEvent();

	mActiveAnimation = FindAnimation(name);
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

function<void()>& Animator::GetEvent(const wstring& name, UINT index)
{
	Events* events = FindEvents(name);

	return events->mEvents[index].mEvent;
}
