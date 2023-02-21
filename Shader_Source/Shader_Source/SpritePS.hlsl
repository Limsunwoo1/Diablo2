#include "globals.hlsli"

float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    
    In.UV = cbxy;
    color = defaultTexture.Sample(anisotropicSampler, In.UV);
    
    return color;
}