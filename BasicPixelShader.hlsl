#include "BasicShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
	//return float4(0,0,0.5,1.0);
	return float4(tex.Sample(smp,input.uv));
}