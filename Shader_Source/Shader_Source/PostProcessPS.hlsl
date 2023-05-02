#include "globals.hlsli"


struct VSIn
{
    float4 Pos : POSITIONT;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

float4 main(VSOut _in) : SV_TARGET
{
    float4 Color = (float4) 0.0f;
    
    // VS_Out ���� ������ SV_Position ���� PixelShader �� �Էµ� �� �ȼ���ǥ�� ��ȯ�ؼ� �Է�
    float2 UV = _in.Pos.xy / resolution;
    
    float2 fAdd = float2(NoiseTexture.Sample(anisortopicSampler, _in.UV + NoiseElapseTime * 0.2).x
                        , NoiseTexture.Sample(anisortopicSampler, _in.UV + float2(0.1f, 0.f) + NoiseElapseTime * 0.2f).x);
    
    fAdd -= fAdd / 2.0f;
    UV += fAdd * 0.05f;
    
    Color = postProcessTexture.Sample(anisortopicSampler, UV);
    
    return Color;
}