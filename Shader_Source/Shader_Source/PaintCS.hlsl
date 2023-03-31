#include "globals.hlsli"

RWTexture2D<float4> tex : register(u0);

// SV_GroupID : 스레드에 속한 그룹의 좌표
// SV_GroupThreadID : 그룹 내에서 스레드의 좌표
// SV_GroupIndex : 그룹 내에서 스레드의 인덱스 좌표(1차원)
// SV_DispatchThreadID " 전체 스레드기준으로 호출된 좌표

#pragma kernel
float random(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453123);
}


[numthreads(32, 32, 1)] // 그룹당 스레드 갯수 (최대 1024개 지정가능)
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