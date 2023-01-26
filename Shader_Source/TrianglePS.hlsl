struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vColor : COLOR;
};

float4 PS_Test(VTX_OUT _in) : SV_Target
{
    // 픽셀 셰이더는 색만 리턴
    return _in.vColor + float4(1.f,0.f,0.f, 0.f);
}