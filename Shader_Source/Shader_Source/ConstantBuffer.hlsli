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
    float elapsedTime; // �����ð�
}

//cbuffer Noise : register(b6)
//{
//    float4 NoiseSize;
//}