#pragma once

// Debug icosphere. Does not load froma file, simply shows how to render stuff.
#include "MaterialOnlyShader.h"

#include <Transform.h>

#include <d3d11.h>
#include <wrl.h>
#include <memory>
using Microsoft::WRL::ComPtr;

namespace sess
{

class DebugMaterialIcosphere
{
public:
	DebugMaterialIcosphere(ComPtr<ID3D11Device> device, MaterialOnlyShader::Material mat, const Vec3& position, const Vec3& scale, float rotationSpeed);
	DebugMaterialIcosphere(const DebugMaterialIcosphere&) = delete;
	~DebugMaterialIcosphere() = default;

	bool Update(float dt);
	bool Render(ComPtr<ID3D11DeviceContext> context, MaterialOnlyShader* shader) const;

protected:
	std::shared_ptr<MaterialOnlyShader::RenderCall> call_;
	MaterialOnlyShader::Material material_;
	
	float rotationSpeed_;
	float rotationAngle_;

	Transform modelTransform_;
};

};