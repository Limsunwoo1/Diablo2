#include "globals.hlsli"

RWTexture2D<float4> tex : register(u0);
RWTexture2D<float4> noise : register(u1);

// SV_GroupID : 스레드에 속한 그룹의 좌표
// SV_GroupThreadID : 그룹 내에서 스레드의 좌표
// SV_GroupIndex : 그룹 내에서 스레드의 인덱스 좌표(1차원)
// SV_DispatchThreadID " 전체 스레드기준으로 호출된 좌표

[numthreads(32, 32, 1)] // 그룹당 스레드 갯수 (최대 1024개 지정가능)
void main( uint3 DTid : SV_DispatchThreadID)
{
    if (1024 <= DTid.x || 1024 <= DTid.y)
        return;
    
    float2 uv = float2(DTid.x / 1024, DTid.y / 1024);
    
    float4 color = noiseTexture.Sample(anisotropicSampler, uv);
    float3 col = color.xyz;
    noise[DTid.xy].xyzw = color;
    
    float r, g, b;
    r = 1.0f - 0.5f;
    g = 1.0f - 0.5f;
    b = 1.0f - 0.5f;
    
    float4 c = float4(col, 1.0f);
    
    
    //tex[DTid.xy] = float4(0.0f, 0.0f, 1.0f, 1.0f);
    //tex[DTid.xy] = float4(r, g, b, 1.0f);
    tex[DTid.xy] = noise[DTid.xy];
}