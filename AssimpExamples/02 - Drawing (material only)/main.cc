#include "DrawingMaterialOnlyApp.h"

int main(int argc, char** argv)
{
	HINSTANCE appInstance = GetModuleHandle(nullptr);
	sess::DrawingMaterialOnlyApp app(appInstance);
	app.Initialize();
	app.Run();

	return EXIT_SUCCESS;
}