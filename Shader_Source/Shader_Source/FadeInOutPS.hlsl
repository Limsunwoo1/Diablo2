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


float4 main(VSOut _In) : SV_Target
{
    float4 color = (float) 0.0f;
    color = defaultTexture.Sample(pointSampler, _In.UV);
    
    color.w = 0.5f;
    return color;
}