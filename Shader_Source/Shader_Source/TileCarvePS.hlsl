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
    //In.UV = In.UV + startUV;
    In.UV = abs(endUV - startUV) * In.UV;
    In.UV += startUV;
    
    color = defaultTexture.Sample(pointSampler, In.UV);
    
    
    if (color.a <= 0.1f)
       discard;
 
    if (color.x > 0.9f && color.z > 0.9f && color.y < 0.1f)
        discard;
    
    color = float4(0.8f, 0.2f, 0.2f, 0.6f);
    
    return color;
}