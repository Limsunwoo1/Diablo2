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
    
    
    
    LightColor _lightColor = (LightColor) 0.0f;
    for (uint i = 0; i < numberOfLight; i++)
    {
        CalculateLight(_lightColor, In.WorldPos.xyz, i);
    }
    color *= _lightColor.diffuse;
    color.w *= In.Color.w;
    
    if (color.a <= 0.1f)
       discard;
 
    if (color.x > 0.9f && color.z > 0.9f && color.y < 0.1f)
        discard;
    
    color *= float4(1.0f, 1.0f, 1.0f, 1.0f);
    color *= float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (OnTile)
    {
        //color *= float4(0.f, 0.f, 1.0f, 1.0f);
        
        color.z = 0.8f;
        
        color.a = 0.6f;
    }
    
    return color;
}