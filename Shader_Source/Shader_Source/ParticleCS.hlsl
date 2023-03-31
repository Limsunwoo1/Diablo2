#include "globals.hlsli"

RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
RWStructuredBuffer<ParticleShader> particleShader : register(u1);

//������ �����尡 ����ȭ�� �ʿ��ϴ�

// 1024�� �ִ� ������ ����
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
            
            // ������ ����ȭ
            // dest ���� exchange ������ �ٲٴµ���
            // ���� ������� �����
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