#include "LightingDefs.hlsli"

// Using a single directional light and a material provided to the object,
//  determine the color of a given pixel fragment using the Phong lighting model

cbuffer PerObject : register(b0)
{
	Material ObjectMaterial;
}

cbuffer PerFrame : register(b1)
{
	float4 CameraPosition;
}

cbuffer PerScene : register(b2)
{
	DirectionalLight DirectionalLight1;
}

// This must mirror the struct used in the vertex buffer
struct PixelIn
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION;
	float4 Normal : NORMAL;
};

float4 main(PixelIn pin) : SV_TARGET
{
	float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
	float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
	float4 specular = float4(0.f, 0.f, 0.f, 0.f);

	ComputeDirectionalLight(ObjectMaterial.AmbientColor, ObjectMaterial.DiffuseColor, ObjectMaterial.SpecularColor,
		DirectionalLight1, normalize(pin.Normal.xyz), normalize(CameraPosition - pin.WorldPosition), ambient, diffuse, specular);

	return saturate(ambient + diffuse + saturate(specular));
}