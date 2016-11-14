#pragma once

#include <DemoApp.h>
#include <FreeCamera.h>

#include "AssimpRoadModel.h"
#include "DebugIcosphere.h"

namespace sess
{

class UVTexturedDemo : public DemoApp
{
public:
	UVTexturedDemo(HINSTANCE appHandle);
	~UVTexturedDemo();

protected:
	// Inherited via DemoApp
	virtual bool InitializeApp() override;
	virtual bool Update(float dt) override;
	virtual bool Render() override;

	virtual LRESULT CALLBACK HandleWin32Message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	MaterialOnlyShader materialOnlyShader_;
	FreeCamera camera_;

	std::shared_ptr<DebugMaterialIcosphere> debugIcosphere_;
	std::shared_ptr<AssimpRoadModel> roadModel_;

	Matrix projMatrix_;

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