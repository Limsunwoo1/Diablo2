struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vColor : COLOR;
};

float4 PS_Test(VTX_OUT _in) : SV_Target
{
    // �ȼ� ���̴��� ���� ����
    return _in.vColor + float4(1.f,0.f,0.f, 0.f);
}