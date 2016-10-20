struct Material
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float4 SpecularColor;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float4 SpecularColor;
	float3 Direction;
	float1 SpecularPower;
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