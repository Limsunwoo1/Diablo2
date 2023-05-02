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

VSOut main(VSIn _In)
{
    //VSOut Out = (VSOut) 0.f;
    //Out.Pos = _In.Pos;
    //Out.Color = _In.Color;
    
    VSOut Out = (VSOut) 0.f;
    
    float4 worldPosition = mul(_In.Pos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(viewPosition, projection);
    
    Out.Pos = ProjPosition;
    Out.WorldPos = worldPosition.xyz;
    Out.Color = _In.Color;
    Out.UV = _In.UV;
    
    return Out;
}