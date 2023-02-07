struct VTX_IN
{
	float4 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUv : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vColor : COLOR;
    float2 vUv : TEXCOORD;
};

// 상수 버퍼
cbuffer Transform : register(b0)
{
    float4 cbPos;
}

cbuffer MaterialData : register(b1)
{
    int     cbiData;
    float   cbfData;
    float2  cbxy;
    float3  cbxyz;
    float4  cbxyzw;
    matrix  cbmat;
}

SamplerState PointSamler : register(s0);
SamplerState LinearSamler : register(S1);
SamplerState AnisotropSampler : register(S2);

VTX_OUT VS_Test(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;

    output.vPos = float4(_in.vPos + cbPos);
    output.vColor = _in.vColor;
    output.vUv = _in.vUv;
    
    return output;
}