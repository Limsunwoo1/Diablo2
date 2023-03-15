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
		Vector2 leftTop;	// 좌측 상단 좌표
		Vector2 size;		// 좌측상단부터 잘라낼 가로 세로의 픽셀 길이
		Vector2 offset;		// 렌더링 위치 조정하기위한 좌표
		Vector2 atlasSize;	// 텍스쳐 이미지의 크기
		float duration;		// 프레임간에 시간 간격

		Sprite()
			: leftTop(Vector2::Zero)
			, size(Vector2::Zero)
			, offset(Vector2::Zero)
			, atlasSize(Vector2::Zero)
			, duration(0.1f)
		{

		}
	};

	Animation();
	virtual ~Animation();

	UINT Update();
	void FixedUpdate();
	void Render();

	void Create(const wstring& name, shared_ptr<Texture2D> atlas
				, Vector2 leftTop, Vector2 size, Vector2 offset
				, UINT spriteLenght, float duration);

	void BindShader();
	void Reset();
	void Clear();

	bool IsComplete() { return mbComplete; }
	wstring& AnimationName() { return mAnimationName; }
private:
	Animator* mAnimator;
	wstring mAnimationName;
	shared_ptr<Texture2D> mAtlas;
	vector<Sprite> mSpriteSheet;
	int mIndex;
	float mTime;
	bool mbComplete;
};