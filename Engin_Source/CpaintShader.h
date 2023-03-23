#pragma once
#include "CComputeShader.h"
#include "CTexture2D.h"

using namespace std;

namespace graphics
{
	class PaintShader : public ComputeShader
	{
	public:
		PaintShader();
		virtual ~PaintShader();

		virtual void Binds() override;
		virtual void Clear() override;

		void SetTarget(shared_ptr<Texture2D> texture) { mTarget = texture; }
	private:
		shared_ptr<Texture2D> mTarget;
	};
}

