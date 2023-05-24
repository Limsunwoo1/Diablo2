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
    float4 color = (float) 0.0f;
    
    if (In.UV.x < startUV.x || In.UV.y < startUV.y)
        discard;
    
    if (In.UV.x > endUV.x || In.UV.y > endUV.y)
        discard;
    
    color = defaultTexture.Sample(pointSampler, In.UV);
    
    //if (color.a <= 0.1f)
    //    discard;
 
    
    return color;
}