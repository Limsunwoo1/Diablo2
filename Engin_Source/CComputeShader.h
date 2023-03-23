#pragma once
#include "Graphics.h"
#include "CResource.h"

using namespace Microsoft::WRL;

namespace graphics
{
	class ComputeShader : public Resource
	{
	public:
		ComputeShader();
		virtual ~ComputeShader();

		virtual HRESULT Load(const std::wstring& path) override;

		bool Create(const std::wstring& file, const std::string& funcName);
		void OnExcute();

		virtual void Binds();
		virtual void Clear();

	protected:
		ComPtr<ID3DBlob> mCSBlob;
		ComPtr <ID3D11ComputeShader> mCS;

		UINT mThreadGropCountX;
		UINT mThreadGropCountY;
		UINT mThreadGropCountZ;

		UINT mGroupX;
		UINT mGroupY;
		UINT mGroupZ;
	};
}

