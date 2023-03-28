#include "globals.hlsli"

struct GSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
};

float4 main(GSOut In) : SV_TARGET
{
    float4 outColor = (float4) 0.0f;
    
    outColor = defaultTexture.Sample(anisotropicSampler, In.UV);
    
    return outColor;
}