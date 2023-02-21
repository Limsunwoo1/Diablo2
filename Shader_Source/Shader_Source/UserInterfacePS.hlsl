#include "globals.hlsli"

float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    color = defaultTexture.Sample(anisotropicSampler, In.UV);
    
    return color;
}