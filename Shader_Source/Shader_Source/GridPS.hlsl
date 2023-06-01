#include "globals.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
    
    float2 WorldPos : POSITION;
};

float4 main(VSOut In) : SV_TARGET
{
    float4 Out = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    const float width = 400 * cameraScale.x;
    const float height = 200 * cameraScale.y;
    
    float lean = height / width;
    
    const float2 offset = float2(0.0f, 400.f);
    
    int x = In.WorldPos.x + offset.x;
    int y = In.WorldPos.y + offset.y;
    
    x %= (int)width;
    y %= (int)height;
    
    // 음수 기울기 그리드
    float reverseX = width - x;
    
    if (abs(((float) y / (float) reverseX) - lean) < 0.0025f)
        return Out;
    
    if (abs(((float) y / (float) x) - lean) < 0.0025f)
        return Out;
    
    discard;
    return float4(1.0f, 0.0f, 1.0f, 1.0f);
}

//
//{
// 선 두께
    //    const float thickness = 1.0f;
    
    //const int XLine = (worldX + 1) % width;
    //const int YLine = (worldY + 1) % height;

  //    if (abs(XLine) == thickness)
    //        return Out;
    
    //if (abs(YLine) == thickness)
    //    return Out;
//}