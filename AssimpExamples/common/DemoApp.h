#pragma once

// Base class for a demo application. Since this is a tutorial series on animation specifically,
//  I won't be doing much consideration around color buffer formats, sizes, anti-aliasing, etc.

#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#include <Windows.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace sess
{

class DemoApp
{
public:
	DemoApp(HINSTANCE hInstance, LPCWSTR appName = L"Demo App");
	~DemoApp();

	// Start the demo
	bool Initialize();

	// Begin game loop
	bool Run();

	// Win32 message handling - must be public so it can be accessed by a global method
	// Well, unless I wanted to resort to some weird code. I don't really feel like that though.
	virtual LRESULT CALLBACK HandleWin32Message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitializeWin32();
	bool InitializeD3D();
	virtual bool InitializeApp() = 0;

	virtual bool Update(float dt) = 0;
	virtual bool Render() = 0;

protected:
	// Device. Calls to this object are thread safe. This is used for GPU calls
	//  that do not directly interact with the graphics pipleine - allocating
	//  buffers, creating textures, uploading shader binaries, etc.
	ComPtr<ID3D11Device> device_;

	// Immediate device context. Calls to this object are not thread safe. This
	//  is used for graphics pipeline interaction - binding shaders, binding
	//  buffers to various stages, queueing draw calls, etc.
	ComPtr<ID3D11DeviceContext> context_;
	
	//
	// Win32 Stuff
	//
	HINSTANCE hInstance_; // Handle to the application
	HWND hWnd_; // Handle to the window
	RECT windowSize_; // Size of the window
	LPCWSTR appName_; // Name of the application
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};