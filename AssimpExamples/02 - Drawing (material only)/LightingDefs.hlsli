struct Material
{
	float4 SpecularColor;
	float4 DiffuseColor;
	float4 AmbientColor;
};

struct DirectionalLight
{
	float3 Direction;
	float1 SpecularPower;
	float4 SpecularColor;
	float4 DiffuseColor;
	float4 AmbientColor;
};

void ComputeDirectionalLight(float4 ambientColor, float4 diffuseColor, float4 specularColor,
	DirectionalLight L, float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 specular)
{
	ambient = float4(0.f, 0.f, 0.f, 0.f);
	diffuse = float4(0.f, 0.f, 0.f, 0.f);
	specular = float4(0.f, 0.f, 0.f, 0.f);

	float3 lightVec = -L.Direction;
	ambient = ambientColor * L.AmbientColor;
	float diffuseFactor = dot(lightVec, normal);

	[flatten]
	if (diffuseFactor > 0.f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.f), specularColor.w);
		diffuse = diffuseFactor * diffuseColor * L.DiffuseColor;
		specular = specFactor * specularColor * L.SpecularColor;
	}
}