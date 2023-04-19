#include "globals.hlsli"

float4 main(float4 position : SV_Position) : SV_TARGET
{
    if (cbiData > 0)
        return float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    return float4(0.0f, 1.0f, 0.0f, 1.0f);
}