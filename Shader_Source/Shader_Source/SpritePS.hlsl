#include "globals.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    
    //In.UV = cbxy;
    color = defaultTexture.Sample(anisotropicSampler, In.UV);
    float4 pp = float4(1.0, 0.f, 1.0f, 1.0f);
    color.xyz += pp.xyz;
    return color;
}