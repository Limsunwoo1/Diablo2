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
            InterlockedExchange(particleShader[0].gActiveCount, exChange, exChange);
            
            if (originValue == exChange)
            {
                ParticleBuffer[DTid.x].active = 1;
                break;
            }
        }
    }
    else // active == 1
    {
        ParticleBuffer[DTid.x].position +=
        ParticleBuffer[DTid.x].direction * ParticleBuffer[DTid.x].speed * delta;
    }
}