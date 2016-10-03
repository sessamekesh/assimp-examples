#include "SetupApp.h"

int main(int argc, char** argv)
{
	HINSTANCE appInstance = GetModuleHandle(nullptr);
	sess::SetupApp app(appInstance);
	app.Initialize();
	app.Run();

	return EXIT_SUCCESS;
}