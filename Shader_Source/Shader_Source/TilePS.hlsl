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
    
    // float2 colrow = float2(5.0f, 37.0f);
    // In.UV.x = (In.UV.x / colrow.x) + (1.f / colrow.x);
    // In.UV.y = (In.UV.y / colrow.y) + (1.f / colrow.y);
    
    //float2 UV = (float2) 0.0f;
    //UV = In.UV / startUV;
    
    //if (In.UV.x < startUV.x || In.UV.y < startUV.y)
    //   discard;
    
    //if (In.UV.x > endUV.x || In.UV.y > endUV.y)
    //   discard;
    
    color = defaultTexture.Sample(pointSampler, In.UV);
    
    //if (color.a <= 0.1f)
    //    discard;
 
    if (color.x > 0.9f && color.z > 0.9f && color.y < 0.1f)
        discard;
    
    return color;
}