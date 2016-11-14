#include "UVTexturedDemo.h"

int main(int argc, char** argv)
{
	HINSTANCE appInstance = GetModuleHandle(nullptr);
	sess::UVTexturedDemo app(appInstance);
	app.Initialize();
	app.Run();

	return EXIT_SUCCESS;
}