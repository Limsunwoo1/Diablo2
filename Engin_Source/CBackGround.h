#pragma once
#include "CGameObject.h"
#include "CTexture2D.h"

using namespace graphics;

class BackGround : public GameObject
{
public:
	BackGround();
	virtual ~BackGround();

	virtual void Initalize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	void SetRenderMode(eRenderingMode type) { mbMode = type; }
	void SetTexture(const std::weak_ptr<Texture2D> tex) { mTexture = tex; }

private:
	std::weak_ptr<Texture2D> mTexture;
	eRenderingMode mbMode;
};