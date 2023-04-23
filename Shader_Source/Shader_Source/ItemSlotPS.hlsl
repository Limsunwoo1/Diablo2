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
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 color = (float) 0.0f;
    color = defaultTexture.Sample(pointSampler, In.UV);
    
    color = cbxyzw;
    return color;
}