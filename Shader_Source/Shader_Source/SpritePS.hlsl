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
        //float2 colrow = float2(3.0f, 3.0f);
        //In.UV.x = (In.UV.x / colrow.x) + (1.f / colrow.x);
        //In.UV.y = (In.UV.y / colrow.y) + (1.f / colrow.y);
        
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
    
    // noise, pater burn effect
    // if(0.0f < NoiseTime)
    //{
       // color.z -= NodseTexture.Sample(pointSampler, In.UV).x * 0.25 *  (10.0f - NoiseTime);
    //}
    
    if (color.a <= 0.1f)
        discard;
    
    
    LightColor _lightColor = (LightColor) 0.0f;
    for (uint i = 0; i < numberOfLight; i++)
    {
        CalculateLight(_lightColor, In.WorldPos.xyz, i);
    }
    color *= _lightColor.diffuse;
    color.w *= In.Color.w;
    
    if (elementType == 1)
    {
        color.xyz *= float3(0.8f, 0.2f, 0.2f);

    }
    else if (elementType == 2)
    {
        color.xyz *= float3(0.2f, 0.2f, 0.8f);
    }
    else if (elementType == 3)
    {
        color.xyz *= float3(1.0f, 1.0f, 0.0f);
    }
    
     return color;
}