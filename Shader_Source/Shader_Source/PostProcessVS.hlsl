#include "globals.hlsli"


struct VSIn
{
    float4 Pos : POSITIONT;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
};

VSOut main(VSIn _in)
{
    VSOut output = (VSOut) 0.0f;
    
    // output = mul(float4(_in.pos.xyz, 1.0f), world);
    
    float4 pos = mul(float4(_in.Pos), world);
    float4 viewPos = mul(float4(pos), view);
    float4 projPos = mul(float4(viewPos), projection);
    
    output.Pos = projPos;
    output.UV = _in.UV;
    
    return output;
}