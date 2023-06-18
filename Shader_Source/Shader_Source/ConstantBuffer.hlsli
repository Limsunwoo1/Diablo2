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
    float bitepadding;
    
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

cbuffer Animation : register(b4)
{
    float2 leftTop;
    float2 spriteSize;
    float2 offset;
    float2 atlasSize;
    
    uint animationType;
    uint elementType;
}
    
cbuffer NumberOfLight : register(b5)
{
    uint numberOfLight;
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
    float4 worldPosition;
    float4 startColor;
    float4 startSize;
    
    uint maxParticles;
    uint simulationSpace;
    float radius;
    float startSpeed;
    
    float startLifeTime;
    float delta;
    float elapsedTime; // 누적시간
    int padding_4;
}

//cbuffer Noise : register(b8)
//{
//    float4 NoiseSize;
//}

cbuffer PlayerData : register(b9)
{
    float hpGauge;
    float mpGauge;
    float RunGauge;
    float expGauge;
    uint gaugeType;
}

cbuffer ItemData : register(b10)
{
    float4 canversColor;
    
    uint stage;
    uint drop;
    uint pick;
    uint onInventory;
}

cbuffer TileData : register(b11)
{
    float2 startUV;
    float2 endUV;
    
    bool OnTile;
    int arrIdx;
}

cbuffer ObjectData : register(b12)
{
    bool OnObject;
    float ObjectAlpha;
}

cbuffer LightingData : register(b13)
{
    uint ElementType;
    bool direction;
    float vScale;
}