
struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

VSOut main(VSIn _In)
{
    VSOut Out = (VSOut) 0.f;
    Out.Pos = _In.Pos;
    Out.Color = _In.Color;
    
    return Out;
}