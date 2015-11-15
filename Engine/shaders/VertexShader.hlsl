
cbuffer draw
{
	float4x4 mvp;
};

struct VS_INPUT
{
	float4 position : POSITION;
};

struct VS_OUTPUT
{
	float4 pos : SV_Position; 
};

VS_OUTPUT main(VS_INPUT input, uint index : SV_InstanceID)
{
	VS_OUTPUT o;
	o.pos = mul(input.position, mvp);
	return o;
}