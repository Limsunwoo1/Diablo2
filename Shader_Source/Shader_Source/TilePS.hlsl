#include "globals.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_Target
{
     float4 color = (float) 0.0f;

    if (tileType == 1) // 2D
    {
        //float2 colrow = float2(4.0f, 1.0f);
        //In.UV.x = (In.UV.x / colrow.x);
        //In.UV.y = (In.UV.y / colrow.y) + (1.f / colrow.y);
        
        //float2 diff = (atlasSize - spriteSize) / 2.0f;
        //float2 UV = (leftTop - diff - offset) + In.UV;

        //if (UV.x < leftTop.x || UV.y < leftTop.y
        //    || UV.x > leftTop.x + spriteSize.x
        //    || UV.y > leftTop.y + spriteSize.y)
        //    discard;
        
        float x =In.UV.x / 4.f + leftTop.x;
        float y = In.UV.y / 1.f + leftTop.y;
        
        float2 UV = float2(x, y);
        
        color = atlasTexture.Sample(pointSampler, UV);
    }
    else
    {
        In.UV = abs(endUV - startUV) * In.UV;
        In.UV += startUV;
    
        color = defaultTexture.Sample(pointSampler, In.UV);
    }
    
    
    //In.UV = In.UV + startUV;
    
    // float2 colrow = float2(5.0f, 37.0f);
    // In.UV.x = (In.UV.x / colrow.x) + (1.f / colrow.x);
    // In.UV.y = (In.UV.y / colrow.y) + (1.f / colrow.y);
    
    //float2 UV = (float2) 0.0f;
    //UV = In.UV / startUV;
    
    //if (In.UV.x < startUV.x || In.UV.y < startUV.y)
    //   discard;
    
    //if (In.UV.x > endUV.x || In.UV.y > endUV.y)
    //   discard;
    
    //color = defaultTexture.Sample(pointSampler, In.UV);
    
    
    
    LightColor _lightColor = (LightColor) 0.0f;
    for (uint i = 0; i < numberOfLight; i++)
    {
        CalculateLight(_lightColor, In.WorldPos.xyz, i);
    }
    color *= _lightColor.diffuse;
    color.w *= In.Color.w;
    
    if (color.a <= 0.1f)
       discard;
 
    if (color.x > 0.9f && color.z > 0.9f && color.y < 0.1f)
        discard;
    
    color *= float4(1.0f, 1.0f, 1.0f, 1.0f);
    color *= float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (OnTile && arrIdx >= 0)
    {
        float sizex = endUV.x - startUV.x;
        float sizey = endUV.y - startUV.y;
    
        float2 sUv = startUV;
        sUv.x += sizex * 0.5f;
        sUv.y += sizey * 0.5f;
        
        if (arrIdx == 0)
            sUv.y += sizey * 0.25f;
        else if (arrIdx == 1)
            sUv.x += sizex * 0.25f;
        else if (arrIdx == 2)
            sUv.x -= sizex * 0.25f;
        else if (arrIdx == 3)
            sUv.y -= sizey * 0.25f;
    
    // 기울기
        float fslope = (sizey * 0.5f) / (sizex * 0.5f);
        float fSlope[4] =
        {
            fslope,
		-fslope,
		fslope,
		-fslope,
        };

	// 마름모 정점
        float2 vVertex[4] =
        {
            { sUv.x, sUv.y + (sizey * 0.25f) },
            { sUv.x + (sizex * 0.25f), sUv.y },
            { sUv.x, sUv.y - (sizey * 0.25f) },
            { sUv.x - (sizex * 0.25f), sUv.y },
        };

	// 절편
        float fY_Intercept[4] =
        {
            vVertex[0].y - (fSlope[0] * vVertex[0].x),
		vVertex[1].y - (fSlope[1] * vVertex[1].x),
		vVertex[2].y - (fSlope[2] * vVertex[2].x),
		vVertex[3].y - (fSlope[3] * vVertex[3].x),
        };

	//0 = ax + b - y;

        float tset[4] =
        {
            fSlope[0] * In.UV.x + fY_Intercept[0] - In.UV.y,
		fSlope[1] * In.UV.x + fY_Intercept[1] - In.UV.y,
		fSlope[2] * In.UV.x + fY_Intercept[2] - In.UV.y,
		fSlope[3] * In.UV.x + fY_Intercept[3] - In.UV.y
        };

        if (0 < fSlope[0] * In.UV.x + fY_Intercept[0] - In.UV.y &&
		0 < fSlope[1] * In.UV.x + fY_Intercept[1] - In.UV.y &&
		0 > fSlope[2] * In.UV.x + fY_Intercept[2] - In.UV.y &&
		0 > fSlope[3] * In.UV.x + fY_Intercept[3] - In.UV.y)
        {
            color.z = 0.8f;
        
            color.a = 0.6f;
        }
    }
    
    return color;
}