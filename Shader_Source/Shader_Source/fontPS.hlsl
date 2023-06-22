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
    
    float x = In.UV.x / 10.f + ImageStartUV.x;
    float y = In.UV.y;
    
    float2 UV = float2(x, y);
    
    //if (In.UV.x < ImageStartUV.x || In.UV.x > ImageEndUV.x)
    //    discard;
    
    color = defaultTexture.Sample(pointSampler, UV);
    
    if (color.a <= 0.1f)
        discard;
    
    return color;
}