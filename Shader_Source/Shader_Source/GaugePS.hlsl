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
    float playerGauge = 0.0f;
    
    if (gaugeType == 1)
    {
        playerGauge = RunGauge;
    }
    else if (gaugeType == 2)
    {
        playerGauge = hpGauge;

    }
    else if (gaugeType == 3)
    {
        playerGauge = mpGauge;

    }
    
    if (gaugeType == 5)
    {
        playerGauge = monsterHP;
    }
    
    float gauge = 1.0f - playerGauge;
    float renderGauge = 1.0f - gauge;
    float2 uv = In.UV;
    
    if (playerGauge <= In.UV.x)
        discard;
    
    float4 color = (float) 0.0f;
    color = defaultTexture.Sample(pointSampler, In.UV);
    
    if (color.a <= 0.1f)
        discard;
    
    if (gaugeType == 5)
    {
        color.a = 0.8f;
    }
    
     return color;
}