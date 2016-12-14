// Vertex buffer to process vertices with only basic information -
//  position and normal only

//
// STRUCT DEFS
//  Here I define the input and output formats of this shader
//
struct VertexIn
{
	float4 Position : POSITION;
	float4 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct PixelIn
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION;
	float4 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

//
// CBUFFERS
//  Here I set up the buffers that will hold the (vertex) shader globals
//  Transformation matrices, for example
//
cbuffer PerObject : register(b0)
{
	matrix mModel;
};

cbuffer PerFrame : register(b1)
{
	matrix mView;
	matrix mProj;
};

PixelIn main(VertexIn vin)
{
	PixelIn vout;

	// Screen space coordinate: model coord -> world coord -> view coord -> screen cord
	vout.Position = mul(vin.Position, mModel);
	vout.Position = mul(vout.Position, mView);
	vout.Position = mul(vout.Position, mProj);

	// World space coordinate: model coord -> world coord
	vout.WorldPosition = mul(vin.Position, mModel);

	// World space normal: model normal -> world normal
	vout.Normal = mul(vin.Normal, mModel);

	vout.UV = vin.UV;

	return vout;
}