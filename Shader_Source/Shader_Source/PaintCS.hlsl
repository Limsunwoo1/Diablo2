#include "globals.hlsli"

RWTexture2D<float4> tex : register(u0);
RWTexture2D<float4> noise : register(u1);

// SV_GroupID : �����忡 ���� �׷��� ��ǥ
// SV_GroupThreadID : �׷� ������ �������� ��ǥ
// SV_GroupIndex : �׷� ������ �������� �ε��� ��ǥ(1����)
// SV_DispatchThreadID " ��ü ������������� ȣ��� ��ǥ

[numthreads(32, 32, 1)] // �׷�� ������ ���� (�ִ� 1024�� ��������)
void main( uint3 DTid : SV_DispatchThreadID, uint DIndex : SV_GroupIndex)
{
    if (1024 <= DTid.x || 1024 <= DTid.y)
        return;
    
    //float2 uv = float2((DTid.x / 1024) * time, (DTid.y / 1024) * time);
    /*uv.x = uv.x * time;
    uv.y = uv.y * Time;*/
    float fcolor = DIndex * deltatime;
    float4 color = noiseTexture.SampleLevel(anisotropicSampler, float2(fcolor, fcolor), 0);
    /*float3 col = color.xyz;
    
    float r, g, b;
    r = 1.0f - 0.5f;
    g = 1.0f - 0.5f;
    b = 1.0f - 0.5f;*/
    
    //float4 c = float4(col, 1.0f);
    
    //tex[DTid.xy] = float4(0.0f, 0.0f, 1.0f, 1.0f);

    tex[DTid.xy] = float4(color.xyz, 1.0f);
}