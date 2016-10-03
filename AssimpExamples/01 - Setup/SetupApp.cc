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
	return true;
}

};