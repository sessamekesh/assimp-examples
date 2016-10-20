#include "DrawingMaterialOnlyApp.h"
#include <Color.h>

#include <iostream>

namespace sess
{

DrawingMaterialOnlyApp::DrawingMaterialOnlyApp(HINSTANCE appHandle)
	: DemoApp(appHandle, L"Demo - Drawing with Materials Only")
	, shader_()
	, camera_(Vec3::Zero, Vec3::UnitZ, Vec3::UnitY)
	, projMatrix_(PerspectiveLH(Radians(85.f), (windowSize_.right - windowSize_.left) / (float)(windowSize_.bottom - windowSize_.top), 0.1f, 100.f))
	, debugIcosphere_(nullptr)
{}

DrawingMaterialOnlyApp::~DrawingMaterialOnlyApp()
{}

//
// Overrides
//
bool DrawingMaterialOnlyApp::InitializeApp()
{
	std::future<bool> shaderLoaded = shader_.Initialize(device_);

	debugIcosphere_ = std::make_shared<DebugMaterialIcosphere>
	(
		device_,
		MaterialOnlyShader::Material(Color::Palette::Red.withAlpha(0.8f), Color::Palette::Red, Color::Palette::Red.clampAndScale(0.2f)),
		Vec3(0.f, 2.f, 5.f),
		Vec3::Ones,
		0.2f
	);

	if (shaderLoaded.get() == false)
	{
		std::cerr << "Failed to load material only shader in material only app" << std::endl;
		return false;
	}

	MaterialOnlyShader::DirectionalLight sun
	(
		Vec3(2.f, -0.6f, 3.f).Normal(),
		Color::Palette::CreamIGuess,
		Color::Palette::CreamIGuess,
		Color::Palette::CreamIGuess
	);
	shader_.SetSunLight(sun);

	return true;
}

bool DrawingMaterialOnlyApp::Update(float dt)
{
	debugIcosphere_->Update(dt);
	return true;
}

bool DrawingMaterialOnlyApp::Render()
{
	float colors[4];
	Color::Palette::Indigo.packAsFloatArray( colors );
	context_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());
	context_->ClearRenderTargetView(renderTargetView_.Get(), colors);
	context_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0x00);
	context_->RSSetViewports(1, &viewport_);

	shader_.SetCameraPosition(camera_.GetPosition());
	shader_.SetViewTransform(camera_.GetViewMatrix());
	shader_.SetProjectionTransform(projMatrix_);

	debugIcosphere_->Render(context_, &shader_);

	swapChain_->Present(1, 0x00);

	return true;
}

};