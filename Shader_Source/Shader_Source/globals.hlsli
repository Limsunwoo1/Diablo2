#include "Light.hlsli"
#include "Particle.hlsli"

cbuffer Transform : register(b0)
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;
}

cbuffer MaterialData : register(b1)
{
    int cbiData;
    float cbfData;
    float2 cbxy;
    float3 cbxyz;
    float4 cbxyzw;
    matrix cbmat;
}


cbuffer Grid : register(b2)
{
    float4 cameraPosition;
    float2 cameraScale;
    float2 resolution;
}

cbuffer FadeData : register(b3)
{
    float alpha;
    float padding1;
    float padding2;
    float padding3;
}

cbuffer NumberOfLight : register(b5)
{
    uint numberOfLight;
}

cbuffer Animation : register(b4)
{
    float2 leftTop;
    float2 spriteSize;
    float2 offset;
    float2 atlasSize;
    
    uint animationType;
}

cbuffer Time : register(b6)
{
    float deltatime;
    float time;
    float padding_;
    float padding_1;
}

cbuffer ParticleSystem : register(b7)
{
    float4 particleColor;
    
    uint elementCount;
    float delta;
}

StructuredBuffer<LightAttribute> lightAttributes : register(t13);
StructuredBuffer<Particle> particleBuffer : register(t15);

SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerState anisotropicSampler : register(s2);

// Defualt Texture
Texture2D defaultTexture : register(t0);

// Defualt Texture
Texture2D noiseTexture : register(t2);

// Atlas Texture
Texture2D atlasTexture : register(t12);

void CalculateLight(in out LightColor  pLightColor, float3 position, int index)
{
    if (0 == lightAttributes[index].type)
    {
        pLightColor.diffuse += lightAttributes[index].color.diffuse;
    }
    else if (1 == lightAttributes[index].type)
    {
        float length = distance(lightAttributes[index].position.xy, position.xy);

        if (length < lightAttributes[index].radius)
        {
            float ratio = 1.0f - (length / lightAttributes[index].radius);
            pLightColor.diffuse += lightAttributes[index].color.diffuse * ratio;

        }
    }
    else
    {
        
    }
}