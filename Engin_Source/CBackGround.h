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

	std::shared_ptr<Texture2D> FindTextuer2D(const std::wstring& name, const std::wstring& path);
	void SetRenderMode(eRenderingMode type) { mbMode = type; }

private:
	std::shared_ptr<class Texture2D> mTextuer2D;
	eRenderingMode mbMode;
};