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
    
    if (animationType == 1) // 2D
    {
        float2 diff = (atlasSize - spriteSize) / 2.0f;
        float2 UV = (leftTop - diff - offset) + (atlasSize * In.UV);

        if (UV.x < leftTop.x || UV.y < leftTop.y
            || UV.x > leftTop.x + spriteSize.x
            || UV.y > leftTop.y + spriteSize.y)
            discard;
        
        color = atlasTexture.Sample(pointSampler, UV);
    }
    else
    {
        color = defaultTexture.Sample(pointSampler, In.UV);
    }
    
    if (stage)
    {
        if (color.a <= 0.2f)
            discard;
        
        return color;
    }
    
    if (!onInventory)
    {
        if (color.w != 0.0f)
            color.w = 0.5f;
        
        return color;
    }
    
    if (color.w == 0.0f)
    {
        if (pick)
        {
            color = canversColor;
        }
    }
    
    if (color.a <= 0.1f)
        discard;
    
    return color;
}