#include "Layer2DHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
	return float4(tex.Sample(smp,input.uv));
}