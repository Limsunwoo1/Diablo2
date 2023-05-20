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

float4 main(VSOut _In) : SV_TARGET
{
    float4 color = (float4) 0.0f;
    
    float halfX = 16.f;
    float halfY = 8.f;
    
    int right = _In.WorldPos.x + halfX;
    int left = _In.WorldPos.x - halfX;
    
    int top = _In.WorldPos.y + halfY;
    int bottom = _In.WorldPos.y - halfY;
    
    //if (_In.WorldPos.x + left )
    
    
    
        return float4(1.0f, 0.0f, 1.0f, 1.0f);
}