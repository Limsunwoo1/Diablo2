#pragma once
#include "GuiWidget.h"

namespace gui
{
	class Game : public Widget
	{
	public:
		Game();
		virtual ~Game();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetCreateTile(bool tile) { mbCreateTile = tile; }
		bool GetCreateTile() { return mbCreateTile; }

		void SetCreateObject(bool obj) { mbCreateObject = obj; }
		bool GetCreateObject() { return mbCreateObject; }

		void SetTex(Texture2D* tex) { mTex = tex; }
		Texture2D* GetTex() { return mTex; }
	private:
		bool mbCreateTile;
		bool mbCreateObject;
		bool mbTickOff;

		Texture2D* mTex;
	};
}

