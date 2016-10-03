#include "SetupApp.h"

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