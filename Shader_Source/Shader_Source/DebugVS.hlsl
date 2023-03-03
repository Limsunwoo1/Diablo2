#include "globals.hlsli"


float4 main( float4 pos : POSITION ) : SV_POSITION
{
    float4 wordPosition = mul(pos, world);
    float4 viewPosition = mul(wordPosition, view);
    float4 ProPosition = mul(viewPosition, projection);
    
    
	return pos;
}