#pragma once
#include "CEntity.h"
#include "Graphics.h"


using namespace Microsoft::WRL;

namespace graphics
{
	class StructedBuffer : public GpuBuffer
	{
	public:
		StructedBuffer();
		~StructedBuffer();

		bool Create(UINT size, UINT stride, eSRVType type, void* data, bool cpuAccess = false);
		void SetData(void* data, UINT bufferCount);
		void GetData(void* data, UINT size = 0);

		void BindSRV(eShaderStage stage, UINT slot);
		void BindUAV(eShaderStage stage, UINT slot);

		UINT GetSize() { return mSize; }
		UINT GetStride() { return mStride; }

		void Clear();
	private:
		void SetDiscription();
		bool CreateBuffer(void* data);
		bool CreateView();
		bool CreateRWBuffer();
	private:
		ComPtr<ID3D11Buffer> mWriteBuffer;
		ComPtr<ID3D11Buffer> mReadBuffer;

		ComPtr<ID3D11ShaderResourceView> mSRV;
		ComPtr<ID3D11UnorderedAccessView> mUAV;

		eSRVType mType;

		UINT mSize;
		UINT mStride;

		UINT mSRVSlot;
		UINT mUAVSlot;
	};
}

