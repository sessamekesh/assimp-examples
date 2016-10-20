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

private:
	MaterialOnlyShader shader_;
	FreeCamera camera_;

	Matrix projMatrix_;

	std::shared_ptr<DebugMaterialIcosphere> debugIcosphere_;
};

};