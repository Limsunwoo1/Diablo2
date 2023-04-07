#include "globals.hlsli"

RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
RWStructuredBuffer<ParticleShader> particleShader : register(u1);

//각각의 스레드가 동기화가 필요하다

// 1024개 최대 스레드 갯수
[numthreads(128, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (elementCount <= DTid.x) 
        return;
    
    if (ParticleBuffer[DTid.x].active == 0)
    {
        while (0 < particleShader[0].gActiveCount)
        {
            int originValue = particleShader[0].gActiveCount;
            int exChange = originValue - 1;
            
            // 스레드 동기화
            // dest 값을 exchange 값으로 바꾸는동안
            // 다음 스레드는 멈춘다
            //InterlockedExchange(particleShader[0].gActiveCount, exChange, exChange);
            InterlockedCompareExchange(particleShader[0].gActiveCount, originValue, exChange, exChange);
            
            if (originValue == exChange)
            {
                ParticleBuffer[DTid.x].active = 1;
                break;
            }
        }
        
        if (ParticleBuffer[DTid.x].active)
        {
            // 랜덤값으로 위치한 방향을 설정해준다
            // 샘플링을 시도할 UV 를 계산해준다
            float3 Random = (float3) 0.0f;
            float2 UV = float2((float) (DTid.x / elementCount), 0.5f);

            UV.x += elapsedTime;
            UV.y += sin((UV.x + elapsedTime) * 3.14592f + 2.0f * 10.0f) * 0.5f;
            
            Random =
            float3
            (
            
                GaussianBlur(UV + float2(0.0f, 0.0f)).x,
                GaussianBlur(UV + float2(0.1f, 0.0f)).x,
                GaussianBlur(UV + float2(0.2f, 0.0f)).x
            );
            
            ParticleBuffer[DTid.x].position.xyz = Random * 1000.0f - 500.0f;
            ParticleBuffer[DTid.x].position.z = 100.f;
        }
    }
    else // active == 1
    {
        //ParticleBuffer[DTid.x].position +=
        //ParticleBuffer[DTid.x].direction * ParticleBuffer[DTid.x].speed * delta;
    }
}