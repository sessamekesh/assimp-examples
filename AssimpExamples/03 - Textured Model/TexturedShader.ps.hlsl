#include "LightingDefs.hlsli"

// Using a single directional light, a diffuse texture, and a material provided to the object,
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

// Texture things
Texture2D DiffuseTexture : register(t0);
SamplerState SampleType
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

// This must mirror the struct used in the vertex buffer
struct PixelIn
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : POSITION;
	float4 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

float4 main(PixelIn pin) : SV_TARGET
{
	float4 ambient = float4(0.f, 0.f, 0.f, 0.f);
	float4 diffuse = float4(0.f, 0.f, 0.f, 0.f);
	float4 specular = float4(0.f, 0.f, 0.f, 0.f);

	float4 textureColor = DiffuseTexture.Sample(SampleType, pin.UV);

	ComputeDirectionalLight(textureColor, textureColor, ObjectMaterial.SpecularColor,
		DirectionalLight1, normalize(pin.Normal.xyz), normalize(CameraPosition - pin.WorldPosition), ambient, diffuse, specular);

	diffuse = diffuse * textureColor;
	ambient = ambient * textureColor;

	return saturate(ambient + diffuse + saturate(specular));
}