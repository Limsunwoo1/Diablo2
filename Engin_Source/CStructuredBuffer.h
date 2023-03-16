#pragma once
#include "CEntity.h"
#include "Graphics.h"


using namespace Microsoft::WRL;

namespace graphics
{
	class StructuredBuffer : public GpuBuffer
	{
	public:
		StructuredBuffer();
		~StructuredBuffer();

		bool Create(UINT size, UINT stride, eSRVType type, void* data);
		void Bind(void* data, UINT bufferCount);
		void SetPipeline(eShaderStage stage, UINT slot);

		UINT GetSize() { return mSize; }
		UINT GetStride() { return mStride; }

	private:
		ComPtr<ID3D11ShaderResourceView> mSRV;
		eSRVType mType;

		UINT mSize;
		UINT mStride;
	};
}

