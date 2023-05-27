#include "globals.hlsli"

struct VS_INPUT
{
    float4 position : POSITION;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
};

float2 IsometricToScreen(float2 isometricPos)
{
    float2 screenPos;
    screenPos.x = (isometricPos.x - isometricPos.y) * 0.5;
    screenPos.y = (isometricPos.x + isometricPos.y) * 0.25;
    return screenPos;
}

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    //float4 worldPosition = mul(input.position, world);
    float2 pos = IsometricToScreen(float2(input.position.xy));
    //worldPosition.xy = pos;
    output.position = float4(pos.x, pos.y, 1.0f,1.0f);
    return output;
}