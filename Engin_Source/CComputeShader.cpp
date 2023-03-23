#include "CComputeShader.h"
#include "CGraphicDevice_DX11.h"

namespace graphics
{
	ComputeShader::ComputeShader()
		: Resource(eResourceType::ComputeShader)
		, mCSBlob(nullptr)
		, mCS(nullptr)
		, mThreadGropCountX(0)
		, mThreadGropCountY(0)
		, mThreadGropCountZ(0)
		, mGroupX(0)
		, mGroupY(0)
		, mGroupZ(0)

	{
	}
	ComputeShader::~ComputeShader()
	{
	}
	HRESULT ComputeShader::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	bool ComputeShader::Create(const std::wstring& file, const std::string& funcName)
	{
		ComPtr<ID3DBlob> mErrorBlob = nullptr;

		// Compute Shader
		std::filesystem::path path = std::filesystem::current_path().parent_path();
		path += "\\Shader_Source\\Shader_Source\\";

		std::wstring shaderPath(path.c_str());
		shaderPath += file;


		D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, funcName.c_str(), "cs_5_0", 0, 0
			, mCSBlob.GetAddressOf()
			, mErrorBlob.GetAddressOf());

		/* if (mErrorBlob)
		 {
			 OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
			 mErrorBlob->Release();
		 }*/

		GetDevice()->CreateComputeShader(mCSBlob->GetBufferPointer()
			, mCSBlob->GetBufferSize()
			, nullptr
			, mCS.GetAddressOf());

		return true;
	}
	void ComputeShader::OnExcute()
	{
		Binds();

		GetDevice()->BindComputeShader(mCS.Get(), nullptr, 0);
		GetDevice()->Dispatch(mThreadGropCountX, mThreadGropCountY, mThreadGropCountZ);

		Clear();
	}
	void ComputeShader::Binds()
	{
	}
	void ComputeShader::Clear()
	{
	}
}
