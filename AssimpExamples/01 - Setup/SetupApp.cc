#include "SetupApp.h"

#include <iostream>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace sess
{

SetupApp::SetupApp(HINSTANCE appHandle)
	: DemoApp(appHandle, L"Demo - ASSIMP initialization")
{}

SetupApp::~SetupApp()
{}

//
// Overrides
//
bool SetupApp::InitializeApp()
{
	auto walkingMan = aiImportFile("../simpleMan2.6.fbx", aiProcessPreset_TargetRealtime_MaxQuality);

	std::cout << "Number of meshes found in file: " << walkingMan->mNumMeshes << std::endl;
	std::cout << "Number of vertices in first mesh: " << walkingMan->mMeshes[0]->mNumVertices << std::endl;

	return true;
}

bool SetupApp::Update(float dt)
{
	return true;
}

bool SetupApp::Render()
{
	float clearColor[] = { 0.f, 0.f, 0.f, 1.f };
	context_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), nullptr);
	context_->ClearRenderTargetView(renderTargetView_.Get(), clearColor);
	
	swapChain_->Present(1, 0x00);

	return true;
}

};