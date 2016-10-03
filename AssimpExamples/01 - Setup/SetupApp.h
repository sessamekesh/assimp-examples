#pragma once

#include <DemoApp.h>

namespace sess
{

class SetupApp : public DemoApp
{
public:
	SetupApp(HINSTANCE appHandle);
	~SetupApp();

protected:
	// Inherited via DemoApp
	virtual bool InitializeApp() override;
	virtual bool Update(float dt) override;
	virtual bool Render() override;
};

};