struct LightColor
{
    float4 diffuse;
    float4 specular;
    float4 ambient;
};

struct LightAttribute
{
    LightColor color;
    float4 position;
    float4 direction;
    
    int type;
    float radius;
    float angle;
    int padding;
};

StructuredBuffer<LightAttribute> lightAttributes : register(t13);
StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);

void CalculateLight(in out LightColor pLightColor, float3 position, int index)
{
    if (0 == lightAttributes[index].type)
    {
        pLightColor.diffuse += lightAttributes[index].color.diffuse;
    }
    else if (1 == lightAttributes[index].type)
    {
        float length = distance(lightAttributes[index].position.xy, position.xy);

        if (length < lightAttributes[index].radius)
        {
            float ratio = 1.0f - (length / lightAttributes[index].radius);
            pLightColor.diffuse += lightAttributes[index].color.diffuse * ratio;

        }
    }
    else
    {
        
    }
}