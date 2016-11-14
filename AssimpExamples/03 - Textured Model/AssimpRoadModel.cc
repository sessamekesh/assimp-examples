#include "AssimpRoadModel.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include <iostream>

namespace sess
{

std::shared_ptr<AssimpRoadModel> AssimpRoadModel::LoadFromFile(const char * fName, ComPtr<ID3D11Device> d3dDevice, const Transform & transform)
{
	const aiScene* scene = aiImportFile(fName, aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene)
	{
		std::cerr << "Could not load file " << fName << std::endl;
		return nullptr;
	}

	// Load all meshes and whatnot
	std::vector<Mesh> meshes;
	meshes.reserve(scene->mNumMeshes);
	for (std::uint32_t meshIdx = 0u; meshIdx < scene->mNumMeshes; meshIdx++)
	{
		aiMesh* mesh = scene->mMeshes[meshIdx];

		std::vector<MaterialOnlyShader::Vertex> verts;
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

		MaterialOnlyShader::Material meshMaterial
		(
			Color(specularColor.r, specularColor.g, specularColor.b, shininess), // Specular
			Color(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a), // Diffuse
			Color(ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a) // Ambient
		);

		for (std::uint32_t vertIdx = 0u; vertIdx < mesh->mNumVertices; vertIdx++)
		{
			aiVector3D vert = mesh->mVertices[vertIdx];
			aiVector3D norm = mesh->mNormals[vertIdx];

			verts.push_back
			(
				MaterialOnlyShader::Vertex
				(
					Vec3(vert.x, vert.y, vert.z),
					Vec3(norm.x, norm.y, norm.z)
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

		MaterialOnlyShader::RenderCall call(d3dDevice, verts, indices);

		meshes.push_back({ call, meshMaterial });
	}

	return std::make_shared<AssimpRoadModel>(meshes, transform);
}

bool AssimpRoadModel::Update(float dt)
{
	return true;
}

bool AssimpRoadModel::Render(ComPtr<ID3D11DeviceContext> context, MaterialOnlyShader* shader) const
{
	shader->SetModelTransform(transform_.GetTransformMatrix());

	for (auto&& mesh : meshes_)
	{
		shader->SetObjectMaterial(mesh.Material);
		shader->Render(context, mesh.Call);
	}

	return true;
}

AssimpRoadModel::AssimpRoadModel(const std::vector<Mesh>& meshes, const Transform& transform)
	: meshes_(meshes)
	, transform_(transform)
{}


};