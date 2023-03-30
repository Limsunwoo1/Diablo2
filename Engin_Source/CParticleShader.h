#pragma once
#include "CComputeShader.h"
#include "CStructedBuffer.h"

using namespace std;

class Texture2D;

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