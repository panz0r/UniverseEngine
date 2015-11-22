
cbuffer meep : register(b4)
{
	float4 test;
};

struct PS_INPUT
{
	float4 pos : SV_Position;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return test;
}