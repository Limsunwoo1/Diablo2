#include "globals.hlsli"

RWStructuredBuffer<Particle> _ParticleBuffer : register(u1);

[numthreads(128, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if (elementCount <= DTid.x) 
        return;
    
    _ParticleBuffer[DTid.x].position += _ParticleBuffer[DTid.x].direction
    * _ParticleBuffer[DTid.x].speed * delta;
}