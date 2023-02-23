#include "globals.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
};


float4 main(VSOut _In) : SV_Target
{
    
    
    return float4(0.0f, 0.0f, 0.0f, delta);
}