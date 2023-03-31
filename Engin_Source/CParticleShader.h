#pragma once
#include "CComputeShader.h"
#include "CStructedBuffer.h"
#include "CTexture2D.h"

using namespace std;


namespace graphics
{
	class ParticleShader : public ComputeShader
	{
	public:
		ParticleShader();
		virtual ~ParticleShader();

		virtual void Binds() override;
		virtual void Clear() override;

		void SetStructedBuffer(StructedBuffer* buffer);

	private:
		StructedBuffer* mBuffer;
	};

}