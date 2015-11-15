
struct PS_INPUT
{
	float4 pos : SV_Position;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return float4(1,0,0,1);
}