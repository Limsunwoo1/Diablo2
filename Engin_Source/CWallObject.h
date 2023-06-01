#pragma once
#include "CGameObject.h"
#include "CTexture2D.h"


using namespace Math;
using namespace graphics;

class WallObject : public GameObject
{
public:
	WallObject();
	virtual ~WallObject();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	void SetWidthHeigth(Vector2 size);
	void SetOffset(Vector2 offset);

	Vector2 GetWidthHeight() { return mWidthHeight; }
	Vector2 GetOffset() { return mOffset; }

	void SetTexture(std::weak_ptr<class graphics::Texture2D> tex) { mTexture = tex; }
	void SetMaterial(std::weak_ptr<class Material> mater) { mMaterial = mater; }

	std::weak_ptr<graphics::Texture2D> GetTexture2D() { return mTexture; }

	void SetWallType(eWallType type);
	eWallType GetWallType() { return mWallType; }
	
private:
	Vector2 mWidthHeight;
	Vector2 mOffset;

	UINT mScreenIndexX;
	UINT mScreenIndexY;

	std::weak_ptr<class Material> mMaterial;
	std::weak_ptr<graphics::Texture2D> mTexture;

	eWallType mWallType;

	float mAlpha;
	bool mbUpdate;
	bool mbOnObject;
};

