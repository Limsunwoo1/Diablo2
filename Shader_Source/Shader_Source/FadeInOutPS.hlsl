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


float4 main(VSOut _In) : SV_Target
{
    float4 color = (float) 0.0f;
    
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
    color = _In.Color;
    
    LightColor _lightColor = (LightColor) 0.0f;
    for (uint i = 0; i < numberOfLight; i++)
    {
        CalculateLight(_lightColor, _In.WorldPos.xyz, i);
    }
    color *= _lightColor.diffuse;
    color.w *= _In.Color.w;
    
    return color;
}