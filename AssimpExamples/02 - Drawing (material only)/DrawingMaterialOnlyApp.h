#pragma once

#include <DemoApp.h>
#include <FreeCamera.h>

#include "MaterialOnlyShader.h"
#include "DebugIcosphere.h"

namespace sess
{

class DrawingMaterialOnlyApp : public DemoApp
{
public:
	DrawingMaterialOnlyApp(HINSTANCE appHandle);
	~DrawingMaterialOnlyApp();

protected:
	// Inherited via DemoApp
	virtual bool InitializeApp() override;
	virtual bool Update(float dt) override;
	virtual bool Render() override;

	virtual LRESULT CALLBACK HandleWin32Message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	MaterialOnlyShader shader_;
	FreeCamera camera_;

	Matrix projMatrix_;

	std::shared_ptr<DebugMaterialIcosphere> debugIcosphere_;

	struct
	{
	public:
		bool W_Pressed;
		bool A_Pressed;
		bool S_Pressed;
		bool D_Pressed;

		bool Left_Pressed;
		bool Right_Pressed;
	} inputState_;
};

};