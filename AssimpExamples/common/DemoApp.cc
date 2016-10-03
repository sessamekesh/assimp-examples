#include <DemoApp.h>

#include <chrono>
#include <iostream>

namespace sess
{

// Hold onto a global (well, to this file) instance of the demo application used.
//  When a new one is created, issue a warning if one already exists.
DemoApp* instance_;

DemoApp::DemoApp(HINSTANCE hInstance, LPCWSTR appName)
	: device_(nullptr)
	, context_(nullptr)
	, hInstance_(hInstance)
	, hWnd_(nullptr)
	, windowSize_({ 100, 100, 400, 400 })
	, appName_(appName)
{
	if (instance_)
	{
		std::cout << "WARNING: Application instance already exists! May result in memory leaks / unpredictable behavior" << std::endl;
	}

	instance_ = this;
}

DemoApp::~DemoApp()
{}

bool DemoApp::Initialize()
{
	if (!InitializeWin32())
	{
		std::cerr << "Failed to initialize Win32! Exiting..." << std::endl;
		return false;
	}

	if (!InitializeD3D())
	{
		std::cerr << "Failed to initialize D3D! Exiting..." << std::endl;
		return false;
	}

	if (!InitializeApp())
	{
		std::cerr << "Failed to initialize application! Exiting..." << std::endl;
		return false;
	}

	std::cout << "Successfully initialized demo!" << std::endl;
	return true;
}

bool DemoApp::Run()
{
	if (!hWnd_)
	{
		std::cerr << "Could not run demo appliation - window has not been initialized" << std::endl;
		return false;
	}

	if (!device_)
	{
		std::cerr << "Could not run demo application - D3D has not been initialized" << std::endl;
		return false;
	}

	MSG msg = {};
	std::chrono::high_resolution_clock::time_point lastFrame = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point now;
	float msTimeElapsed = 0.f;

	while (true)
	{
		// Check to see if there is a message in the queue
		if (PeekMessage(&msg, nullptr, 0x00, 0x00, PM_REMOVE))
		{
			// If there is, translate and dispatch via OS to our window listener
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

		now = std::chrono::high_resolution_clock::now();
		msTimeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - lastFrame).count() / 1000.f;
		lastFrame = now;
		if (!Update(msTimeElapsed))
		{
			std::cout << "Failed to update frame!" << std::endl;
		}

		if (!Render())
		{
			std::cout << "Failed to render frame!" << std::endl;
		}
	}
}

bool DemoApp::InitializeWin32()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = appName_;
	wc.hInstance = hInstance_;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = wc.hIcon;
	
	RegisterClassEx(&wc);

	hWnd_ = CreateWindow
	(
		appName_, appName_,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX,
		windowSize_.left, windowSize_.top,
		(windowSize_.right - windowSize_.left),
		(windowSize_.bottom - windowSize_.top),
		nullptr, nullptr, hInstance_,
		nullptr
	);
	
	if (!hWnd_)
	{
		std::cerr << "Failed to create window!" << std::endl;\
		std::cerr << GetLastError() << std::endl;
		return false;
	}

	ShowWindow(hWnd_, SW_SHOWDEFAULT);
	return true;
}

bool DemoApp::InitializeD3D()
{
	UINT creationFlags = 0x00;
#if defined(DEBUG) | defined(_DEBUG)
	// Set an extra flag used in creating the device that specifies to DirectX
	//  to use debug information. This is used in conjunction with the debug
	//  interface seen below. This includes reference counting for COM objects,
	//  and more advanced output regarding pipeline errors, bindings, etc.
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = {};
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	hr = D3D11CreateDevice
	(
		nullptr, D3D_DRIVER_TYPE_HARDWARE,
		nullptr, creationFlags,
		&featureLevel, 1u, D3D11_SDK_VERSION,
		&device_, nullptr, &context_
	);

	if (FAILED(hr))
	{
		std::cerr << "Failed to create D3D11 device! Error code: " << hr << std::endl;
		return false;
	}

#if defined(DEBUG) | defined(_DEBUG)
	// Moar debug (see above comments)
	ComPtr<ID3D11Debug> debugInterface;
	hr = device_.Get()->QueryInterface(IID_PPV_ARGS(&debugInterface));
	if (FAILED(hr))
	{
		// Inability to use the debug interface is not a fatal error - we can continue
		//  without it (hopefully - this is disabled on release builds!)
		std::cout << "Unable to use debug interface, even on debug build. Sorry." << std::endl;
	}
#endif

	return true;
}

LRESULT CALLBACK DemoApp::HandleWin32Message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_QUIT || msg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return EXIT_SUCCESS;
	}
	else if (instance_)
	{
		return instance_->HandleWin32Message(hWnd, msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

};