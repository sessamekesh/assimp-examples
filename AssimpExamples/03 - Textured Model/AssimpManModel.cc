#include "AssimpManModel.h"

#include <lodepng.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <iostream>

namespace sess
{

std::shared_ptr<AssimpManModel> AssimpManModel::LoadFromFile(const char* fName, const char* textureFilename, ComPtr<ID3D11Device> d3dDevice, ComPtr<ID3D11DeviceContext> d3dDeviceContext, const Transform& transform)
{
	const aiScene* scene = aiImportFile(fName, aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene)
	{
		std::cerr << "Could not load file " << fName << std::endl;
		return nullptr;
	}

	//
	// Load image with LodePNG
	//
	std::vector<unsigned char> textureData; // Raw pixel data
	std::uint32_t imageWidth, imageHeight; // Image metadata

	// Decode image with LodePNG
	std::uint32_t decodeError = lodepng::decode(textureData, imageWidth, imageHeight, textureFilename);

	// Flip all values on Y
	for (int row = 0; row < imageHeight / 2u; row++)
	{
		for (int col = 0; col < imageWidth; col++)
		{
			int topPixelId = row * imageWidth + col;
			int botPixelId = (imageHeight - row - 1) * imageWidth + col;
			for (int component = 0; component < 4; component++)
			{
				unsigned char temp = textureData[topPixelId * 4 + component];
				textureData[topPixelId * 4 + component] = textureData[botPixelId * 4 + component];
				textureData[botPixelId * 4 + component] = temp;
			}
		}
	}

	//
	// ... Done loading image. Lode Vandevenne, you're AWESOME dude
	//

	TexturedShader::Texture manTexture(d3dDevice, d3dDeviceContext, textureData, imageWidth, imageHeight);

	// Load all meshes and whatnot
	std::vector<Mesh> meshes;
	meshes.reserve(scene->mNumMeshes);
	for (std::uint32_t meshIdx = 0u; meshIdx < scene->mNumMeshes; meshIdx++)
	{
		aiMesh* mesh = scene->mMeshes[meshIdx];

		std::vector<TexturedShader::Vertex> verts;
		verts.reserve(mesh->mNumVertices);

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiColor4D specularColor;
		aiColor4D diffuseColor;
		aiColor4D ambientColor;
		float shininess;

		aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specularColor);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
		aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
		aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);

		TexturedShader::Material meshMaterial
		(
			Color(specularColor.r, specularColor.g, specularColor.b, shininess), // Specular
			Color(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a), // Diffuse
			Color(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a) // Ambient
		);

		for (std::uint32_t vertIdx = 0u; vertIdx < mesh->mNumVertices; vertIdx++)
		{
			aiVector3D vert = mesh->mVertices[vertIdx];
			aiVector3D norm = mesh->mNormals[vertIdx];
			aiVector3D uv = mesh->mTextureCoords[0][vertIdx];

			verts.push_back
			(
				TexturedShader::Vertex
				(
					Vec3(vert.x, vert.y, vert.z),
					Vec3(norm.x, norm.y, norm.z),
					uv.x, uv.y
				)
			);
		}

		std::vector<std::uint32_t> indices;
		indices.reserve(mesh->mNumFaces * 3u);
		for (std::uint32_t faceIdx = 0u; faceIdx < mesh->mNumFaces; faceIdx++)
		{
			indices.push_back(mesh->mFaces[faceIdx].mIndices[0u]);
			indices.push_back(mesh->mFaces[faceIdx].mIndices[1u]);
			indices.push_back(mesh->mFaces[faceIdx].mIndices[2u]);
		}

		TexturedShader::RenderCall call(d3dDevice, verts, indices);

		meshes.push_back({ call, meshMaterial });
	}

	return std::make_shared<AssimpManModel>(meshes, transform, manTexture);
}

bool AssimpManModel::Update(float dt)
{
	return true;
}

bool AssimpManModel::Render(ComPtr<ID3D11DeviceContext> context, TexturedShader* shader) const
{
	shader->SetModelTransform(transform_.GetTransformMatrix());
	shader->SetTexture(texture_);

	for (auto&& mesh : meshes_)
	{
		shader->SetObjectMaterial(mesh.Material);
		shader->Render(context, mesh.Call);
	}

	return true;
}

AssimpManModel::AssimpManModel(const std::vector<Mesh>& meshes, const Transform& transform, TexturedShader::Texture texture)
	: meshes_(meshes)
	, transform_(transform)
	, texture_(texture)
{}

};
