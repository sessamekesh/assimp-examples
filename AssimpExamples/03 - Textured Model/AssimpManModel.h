#pragma once

#include <Transform.h>
#include <vector>
#include <memory>

#include "TexturedShader.h"

namespace sess
{

class AssimpManModel
{
public:
	struct Mesh
	{
		TexturedShader::RenderCall Call;
		TexturedShader::Material Material;
	};

public:
	AssimpManModel(const std::vector<Mesh>& meshes, const Transform& transform, TexturedShader::Texture texture);

	static std::shared_ptr<AssimpManModel> LoadFromFile(const char* fName, const char* textureFilename, ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dDeviceContext, const Transform& transform);
	bool Update(float dt);
	bool Render(ComPtr<ID3D11DeviceContext> context, TexturedShader* shader) const;

	AssimpManModel(const AssimpManModel&) = delete;
	~AssimpManModel() = default;

protected:
	std::vector<Mesh> meshes_;
	TexturedShader::Texture texture_;
	Transform transform_;
};

};