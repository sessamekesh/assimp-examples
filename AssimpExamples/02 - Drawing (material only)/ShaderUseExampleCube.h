#pragma once

// This class just shows how things are drawn using this particular code setup.
// A cube is formed here, one that can be drawn

#include "MaterialOnlyShader.h"
#include <Transform.h>

namespace sess
{

class ShaderUseExampleCube
{
public:
	ShaderUseExampleCube();
	ShaderUseExampleCube(const ShaderUseExampleCube&) = delete;
	~ShaderUseExampleCube() = default;

	bool Update(float dt);
	bool Render(ComPtr<ID3D11DeviceContext> context, const MaterialOnlyShader& shader) const;

protected:
	MaterialOnlyShader::RenderCall call_;
	MaterialOnlyShader::Material material_;

	// Spatial data - position, orientation, scale, of this box
	// Using a transform object to make the logic easier
	// Will follow a fun little animation path defined in the Update method
	Transform objectTransform_;
};

};