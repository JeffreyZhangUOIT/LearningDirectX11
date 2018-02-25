
cbuffer world {
	matrix world;
	matrix view;
	matrix proj;
};
struct Input 
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct Output 
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

Output main(Input input) 
{
	Output output;
	//float x = input.position.x;
	//float y = input.position.y;

	output.position = float4(input.position.x, input.position.y, input.position.z, 1);
	output.position = mul(output.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);
	output.color = input.color;
	return output;
}