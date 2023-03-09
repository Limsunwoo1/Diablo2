#pragma once
#include "CEntity.h"
#include "CTexture2D.h"

using namespace Math;
using namespace graphics;
using namespace std;

class Animator;
class Animation : public Entity
{
public:
	struct Sprite
	{
		Vector2 leftTop;
		Vector2 size;
		Vector2 offset;
		float duration;

		Sprite()
			: leftTop(Vector2::Zero)
			, size(Vector2::Zero)
			, offset(Vector2::Zero)
			, duration(0.1f)
		{

		}
	};

	Animation();
	virtual ~Animation();

	void Update();
	void FixedUpdate();
	void Render();

	void Create(const wstring& name, shared_ptr<Texture2D> atlas
				, Vector2 leftTop, Vector2 size, Vector2 offset
				, UINT columnLength, UINT spriteLenght, float duration);

	void BindShader();
	void Reset();
	void Clear();

	bool IsComplete() { return mbComplete; }
	wstring& AnimationName() { return mName; }
private:
	Animator* mAnimator;
	wstring mName;
	shared_ptr<Texture2D> mAtlas;
	vector<Sprite> mSpriteSheet;
	int mIndex;
	float mTime;
	bool mbComplete;
};