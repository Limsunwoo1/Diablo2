#include "globals.hlsli"

float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 color = (float) 0.0f;
    
    color = defaultTexture.Sample(anisotropicSampler, _in.vUV);
    
    return color;
    
    // �ȼ� ���̴��� ���� ����
    //return _in.vColor + float4(1.f,0.f,0.f, 0.f);
}