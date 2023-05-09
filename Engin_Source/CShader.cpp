#include "CShader.h"
#include "CGraphicDevice_DX11.h"
#include "CRenderer.h"

Shader::Shader()
    : Resource(eResourceType::GraphicShader)
    , mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    , mRSType(eRasterizeType::SolidBack)
    , mDSType(eDepthStencilType::Less)
    , mBLType(eBlendType::AlphaBlend)
{
}

Shader::~Shader()
{
    int a = 0;
}

HRESULT Shader::Load(const std::wstring& path)
{
    return E_NOTIMPL;
}

void Shader::Create(graphics::eShaderStage stage, const std::wstring& file, const std::string& funcName)
{
    mErrorBlob = nullptr;

    // Vertex Shader
    std::filesystem::path path = std::filesystem::current_path().parent_path();
    path += "\\Shader_Source\\Shader_Source\\";

    std::wstring shaderPath(path.c_str());
    shaderPath += file;

    if (stage == graphics::eShaderStage::VS)
    {
        D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
                         , funcName.c_str(), "vs_5_0", 0, 0
                         , mVSBlob.GetAddressOf()
                         , mErrorBlob.GetAddressOf());

        if (mErrorBlob)
        {
            OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
            mErrorBlob->Release();
        }

        GetDevice()->CreateVertexShader(mVSBlob->GetBufferPointer()
                                                , mVSBlob->GetBufferSize()
                                                , nullptr
                                                , mVS.GetAddressOf());
    }
    else if (stage == graphics::eShaderStage::PS)
    {
        D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
                         , funcName.c_str(), "ps_5_0", 0, 0
                         , mPSBlob.GetAddressOf()
                         , mErrorBlob.GetAddressOf());

        if (mErrorBlob)
        {
            OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
            mErrorBlob->Release();
        }

        GetDevice()->CreatePixelShader(mPSBlob->GetBufferPointer()
                                               , mPSBlob->GetBufferSize()
                                               , nullptr
                                               , mPS.GetAddressOf());
    }
    else if (stage == graphics::eShaderStage::GS)
    {
        D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
            , funcName.c_str(), "gs_5_0", 0, 0
            , mGSBlob.GetAddressOf()
            , mErrorBlob.GetAddressOf());

         /*if (mErrorBlob)
         {
             OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
             mErrorBlob->Release();
         }*/

        GetDevice()->CreateGeometryShader(mGSBlob->GetBufferPointer()
            , mGSBlob->GetBufferSize()
            , nullptr
            , mGS.GetAddressOf());
    }
}

void Shader::Binds()
{
    // IA
    GetDevice()->BindPrivitiveTopology(mTopology);
    GetDevice()->BindInputLayout(mInputLayout.Get());

    // shader
    GetDevice()->BindVertexShader(mVS.Get(), nullptr, 0);
    GetDevice()->BindHullShader(mHS.Get(), nullptr, 0);
    GetDevice()->BindDomainShader(mDS.Get(), nullptr, 0);
    GetDevice()->BindGeometryShader(mGS.Get(), nullptr, 0);
    GetDevice()->BindPixelShader(mPS.Get(), nullptr, 0);


    Microsoft::WRL::ComPtr<ID3D11RasterizerState>   RS = Renderer::RasterizeState[(UINT)mRSType];
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DS = Renderer::Depth_StencilState[(UINT)mDSType];
    Microsoft::WRL::ComPtr<ID3D11BlendState>        BL = Renderer::BlendState[(UINT)mBLType];

    GetDevice()->BindRasterizerState(RS.Get());
    GetDevice()->BindDepthStencilState(DS.Get());
    GetDevice()->BindBlendState(BL.Get());
}
