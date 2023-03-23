#include "CpaintShader.h"


namespace graphics
{
	PaintShader::PaintShader()
		: ComputeShader()
		, mTarget(nullptr)
	{
	}
	PaintShader::~PaintShader()
	{
	}

	void PaintShader::Binds()
	{
		mTarget->BIndUnorderedAccessView(0);

		mGroupX = mTarget->GetWidth() / mThreadGropCountX + 1;
		mGroupY = mTarget->GetHeight() / mThreadGropCountY + 1;
		mGroupZ = 1;
	}

	void PaintShader::Clear()
	{
		mTarget->ClearUnorderedAccessView(0);
	}
}
