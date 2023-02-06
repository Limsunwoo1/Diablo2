struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

Texture2D defaultTexture : register(t0);

SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerState anisotropicSampler : register(s2);

float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 color = (float) 0.0f;
    
    color = defaultTexture.Sample(anisotropicSampler, _in.vUV);
    
    return color;
    
    // 픽셀 셰이더는 색만 리턴
    //return _in.vColor + float4(1.f,0.f,0.f, 0.f);
}