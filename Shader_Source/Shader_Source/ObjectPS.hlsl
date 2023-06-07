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
    
    color *= float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    //color.a = ObjectAlpha;
    color.a = ObjectAlpha;
    
    if (OnObject)
    {
    //color *= float4(0.f, 0.f, 1.0f, 1.0f);
    
        color.y = 0.6f;
        color.a = 0.6f;
    }
    
    
    return color;
}