#include "BasicShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
	return float4(input.uv,1,1);
}