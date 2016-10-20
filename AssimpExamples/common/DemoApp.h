#pragma once

// Base class for a demo application. Since this is a tutorial series on animation specifically,
//  I won't be doing much consideration around color buffer formats, sizes, anti-aliasing, etc.

#define WIN32_LEAN_AND_MEAN
#include <d3d11.h>
#include <dxgi1_4.h>
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
	
	// DXGI is the DirectX Graphics Infrastructure, and it provides a
	//  lot of really handy utilities for connecting Direct3D to a Win32 window.
	// Swap chain - stores two color buffers. One is actively being drawn to
	//  by Direct3D, and the other is being shown to the user. When drawing is
	//  finished, we switch which one has which job - this shows the next frame
	//  to the user, and uses the old frame for drawing the subsequent frame.
	// The naming is horrible. IDXGISwapChain1 has a little bit more features than
	//  IDXGISwapChain alone. IDXGISwapChain has all of the really important stuff.
	ComPtr<IDXGISwapChain1> swapChain_;

	// If I had a nickel for every different class that has to be used that only
	//  represents a rectangle in this project... I'd have two nickles.
	// The viewport is what part of the surface (in our case, window) that is being
	//  rendered to in the graphics pipeline. For us, this will be the entire window.
	// This could feasibly be a smaller part of the window. For example, a mini-map could
	//  be drawn to a small viewport in the upper-right corner of an RPG style game
	D3D11_VIEWPORT viewport_;

	// The render target will be a color buffer (i.e., picture in GPU memory) to which
	//  our frames will be drawn. The actual buffer(s) will be created by the swap chain.
	// The view is a CPU-side handle, almost a pointer but not quite. When the context
	//  is given this view, it will know how to find the actual buffer, but on the CPU
	//  side this can't be deferenced. The CPU does not directly interact with GPU memory.
	ComPtr<ID3D11RenderTargetView> renderTargetView_;

	// Depth stencil state. Part of the configurable (but not programmable) pipeline that
	//  D3D wants you to have included.
	ComPtr<ID3D11DepthStencilState> depthStencilState_;

	// Rasterizer state. Just another part of the configurable (but not programmable) pipeline
	ComPtr<ID3D11RasterizerState> rasterizerState_;

	// Depth stencil buffer - used to make sure we don't draw things on top of each other
	//  in the wrong order. We're not sure what order the shapes will be drawn in, and it
	//  definitely won't be back-to-front. Unless we get really really lucky, maybe?
	// Anyways, this just makes sure that we don't draw background things in front of foreground
	ComPtr<ID3D11Texture2D> depthStencilBuffer_;

	ComPtr<ID3D11DepthStencilView> depthStencilView_;

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