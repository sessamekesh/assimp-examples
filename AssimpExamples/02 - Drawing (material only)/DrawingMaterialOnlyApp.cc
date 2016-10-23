#include "DrawingMaterialOnlyApp.h"
#include <Color.h>

#include <iostream>

namespace sess
{

DrawingMaterialOnlyApp::DrawingMaterialOnlyApp(HINSTANCE appHandle)
	: DemoApp(appHandle, L"Demo - Drawing with Materials Only")
	, shader_()
	, camera_(Vec3(0.f, 2.f, 0.f), Vec3(0.f, 2.f, 1.f), Vec3::UnitY)
	, projMatrix_(PerspectiveLH(Radians(85.f), (windowSize_.right - windowSize_.left) / (float)(windowSize_.bottom - windowSize_.top), 0.1f, 100.f))
	, debugIcosphere_(nullptr)
	, inputState_({ /* Initialize to all false */ })
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
		MaterialOnlyShader::Material(Color::Palette::Red.withAlpha(0.8f), Color::Palette::Red, Color::Palette::Red.clampAndScale(0.9f)),
		Vec3(0.f, 1.8f, 6.f),
		Vec3::Ones,
		1.1f
	);

	if (shaderLoaded.get() == false)
	{
		std::cerr << "Failed to load material only shader in material only app" << std::endl;
		return false;
	}

	MaterialOnlyShader::DirectionalLight sun
	(
		Vec3(2.f, -1.6f, 3.f).Normal(),
		Color::Palette::PureWhite,
		Color::Palette::PureWhite,
		Color::Palette::PureWhite.clampAndScale(0.15f)
	);
	shader_.SetSunLight(sun);

	return true;
}

bool DrawingMaterialOnlyApp::Update(float dt)
{
	const static float ROTATE_SPEED = 1.8f;
	const static float MOVE_SPEED = 10.f;

	if (inputState_.W_Pressed && !inputState_.S_Pressed)
	{
		camera_.MoveForward(dt / 1000.f * MOVE_SPEED);
	}
	if (inputState_.S_Pressed && !inputState_.W_Pressed)
	{
		camera_.MoveForward(dt / 1000.f * -MOVE_SPEED);
	}
	if (inputState_.A_Pressed && !inputState_.D_Pressed)
	{
		camera_.MoveRight(dt / 1000.f * -MOVE_SPEED);
	}
	if (inputState_.D_Pressed && !inputState_.A_Pressed)
	{
		camera_.MoveRight(dt / 1000.f * MOVE_SPEED);
	}

	if (inputState_.Left_Pressed && !inputState_.Right_Pressed)
	{
		camera_.RotateRight(dt / 1000.f * -ROTATE_SPEED);
	}
	if (inputState_.Right_Pressed && !inputState_.Left_Pressed)
	{
		camera_.RotateRight(dt / 1000.f * ROTATE_SPEED);
	}

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

LRESULT DrawingMaterialOnlyApp::HandleWin32Message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Handle key presses
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'W': inputState_.W_Pressed = true; break;
		case 'A': inputState_.A_Pressed = true; break;
		case 'D': inputState_.D_Pressed = true; break;
		case 'S': inputState_.S_Pressed = true; break;
		case VK_LEFT: inputState_.Left_Pressed = true; break;
		case VK_RIGHT: inputState_.Right_Pressed = true; break;
		case VK_ESCAPE: PostQuitMessage(0); break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'W': inputState_.W_Pressed = false; break;
		case 'A': inputState_.A_Pressed = false; break;
		case 'D': inputState_.D_Pressed = false; break;
		case 'S': inputState_.S_Pressed = false; break;
		case VK_LEFT: inputState_.Left_Pressed = false; break;
		case VK_RIGHT: inputState_.Right_Pressed = false; break;
		}
		break;
		break;
	}

	return DemoApp::HandleWin32Message(hWnd, msg, wParam, lParam);
}

};