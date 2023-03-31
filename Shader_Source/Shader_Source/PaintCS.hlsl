#include "globals.hlsli"

RWTexture2D<float4> tex : register(u0);

// SV_GroupID : �����忡 ���� �׷��� ��ǥ
// SV_GroupThreadID : �׷� ������ �������� ��ǥ
// SV_GroupIndex : �׷� ������ �������� �ε��� ��ǥ(1����)
// SV_DispatchThreadID " ��ü ������������� ȣ��� ��ǥ

#pragma kernel
float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453123);
}


[numthreads(32, 32, 1)] // �׷�� ������ ���� (�ִ� 1024�� ��������)
void main( uint3 DTid : SV_DispatchThreadID, uint DIndex : SV_GroupIndex)
{
    if (1024 <= DTid.x || 1024 <= DTid.y)
        return;
    
    //float2 uv = float2((DTid.x / 1024) * time, (DTid.y / 1024) * time);
    
    //uv.x = uv.x * time;
    //uv.y = uv.y * time;
    
    float u = ((float(DTid.x) / float(DIndex)) + time) * 0.5f;
    float v = ((float(DTid.y) / float(DIndex)) + time) * 0.5f;

    //float fcolor = DIndex * time;
    
    float4 color = noiseTexture.SampleLevel(pointSampler, float2(u, v), 0);
    
    float k = random(float2(u, v));
    color.x *= k;
    k = random(float2(color.x, k));
    color.y *= k;
    k = random(float2(color.y, k));
    color.z *= k;
    
    //float4 c = float4(col, 1.0f);
    //tex[DTid.xy] = float4(0.0f, 0.0f, 1.0f, 1.0f);


    //color.xyz = (time * color.xyz);
    //color.xyz += time;
    //color.xyz *= k;

    tex[DTid.xy] = float4(color.xyz, 1.0f);
    
    //tex[DTid.xy] = float4(time, time, time, 1.0f);
}