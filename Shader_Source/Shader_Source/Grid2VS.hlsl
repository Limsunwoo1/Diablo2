#include "globals.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 WorldPos : POSITION;
};



VSOut main(VSIn _In)
{
    VSOut Out = (VSOut) 0.0f;
    
    float4 worldPosition = mul(_In.Pos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(viewPosition, projection);
    
    Out.Pos = ProjPosition;
    Out.WorldPos = worldPosition;
    
    return Out;
}