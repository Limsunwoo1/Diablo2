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

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
    matrix testworld = world;
    testworld._11 *= cameraScale.x;
    testworld._22 *= cameraScale.x;
    testworld._33 *= cameraScale.x;
    
    float4 worldPosition = mul(In.Pos, testworld);
    float4 viewPosition = mul(worldPosition, view);
    float4 ProjPosition = mul(viewPosition, projection);
    
    Out.WorldPos = worldPosition.xyz;
    Out.Pos = ProjPosition;
    Out.Color = In.Color;
    Out.UV = In.UV;
    
    return Out;
}