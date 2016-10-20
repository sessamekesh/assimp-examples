#pragma once

// IMPORTANT! If you're reading this, pay attention to the comments! The code is carefully
//  separated SPATIALLY according to if it's DirectX stuff or general shader stuff.
// There's a TON of boilerplate code here, and some of it is more complicated than it needs to be.
// The stuff near the top is what you should be concerned with

// This class forms a bridge between the logic and rendering.
//  The method that will be used externally is just the "Render" method,
//  which will VERY CAREFULLY only take in data that is useful
//  across DirectX 9/10/11, OpenGL 2/3/4, and Vulkan

// This shader specifically is for use objects that only have material properties defined.
// Each vertex will have:
// - Position
// - Normal
// Each object (draw call) will have:
// - Camera position
// - Material (diffuse, specular, ambient colors)

#include <d3d11.h>
#include <wrl.h>
#include <future>
#include <vector>
#include <MathExtras.h>

using Microsoft::WRL::ComPtr;

namespace sess
{

class MaterialOnlyShader
{
public:
	// Data structures used by this shader (GRAPHICS API AGNOSTIC)
	struct Vertex
	{
	public:
		Vec3 Position;
		float __one;
		Vec3 Normal;
		float __zero;

	public:
		Vertex(const Vec3& pos, const Vec3& norm)
			: Position(pos), __one(1.f), Normal(norm), __zero(0.f)
		{}
	};

	// Both lights and materials have three color components - ambient, specular, diffuse
	// "Ambient" light/color refers to what color a surface has when it is not exposed directly
	//  to light from the object. Think of an overcast day - everything in this case is lit by
	//  ambient lighting alone. Things in shadow are also lit by ambient lights alone.
	// "Diffuse" light/color refers to the color when a light hits a surface, illuminating it.
	//  Most full-on light is visible as diffuse light.
	// "Specular" light/color refers to the shiny spots on things like water and metallic surfaces
	//  When you look across a lake in the sunset, there's super bright white spots that appear
	//  where the reflection of the sun is. Those are lit primarily by specular light.
	// For further information, read up on the Phong lighting model.
	struct Material
	{
		Material(const Color& specular, const Color& diffuse, const Color& ambient)
			: Specular(specular), Diffuse(diffuse), Ambient(ambient)
		{}
		Color Specular;
		Color Diffuse;
		Color Ambient;
	};

	struct DirectionalLight
	{
		DirectionalLight(const Vec3& direction, const Color& specular, const Color& diffuse, const Color& ambient)
			: Direction(direction), __pad(0.f), Specular(specular), Diffuse(diffuse), Ambient(ambient)
		{}

		Vec3 Direction;
		float __pad; // To keep 16-byte aligned-ness of constant buffer
		Color Specular;
		Color Diffuse;
		Color Ambient;
	};

	// Calls used by this shader to set shader variables (GRAPHICS API AGNOSTIC)
	//  If you want to change any of these values between render calls, these functions
	//  must be called. The render call itself has no notion of any of these things.
	// That also helps in optimizations - DX11/GL/VK all have optimizations that can be
	//  applied by using this sort of pattern. Different optimizations, but optimizations.
	void SetModelTransform(const Matrix& modelTransform);
	void SetViewTransform(const Matrix& viewTransform);
	void SetProjectionTransform(const Matrix& projTransform);
	void SetObjectMaterial(const Material& objectMaterial);
	void SetCameraPosition(const Vec3& cameraPosition);
	void SetSunLight(const DirectionalLight& light);

public:
	// Bridge stuff. The implementation here is graphics API dependent, but the
	//  creation and external use of it isn't.
	
	// RenderCall is the only input to the actual render function
	class RenderCall
	{
	public:
		RenderCall(ComPtr<ID3D11Device> device, const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);

	public:
		// Here's the DirectX specific stuff
		// I could do without this part, but the GPU-side caching of vertices is just
		//  too darn good to forego
		ComPtr<ID3D11Buffer> VertexBuffer;
		ComPtr<ID3D11Buffer> IndexBuffer;
		std::uint32_t NumberOfIndices;
	};

public:
	// Graphics code specific stuff. This is where the DirectX magic happens
	MaterialOnlyShader();
	MaterialOnlyShader(const MaterialOnlyShader&) = delete;
	~MaterialOnlyShader() = default;

	std::future<bool> Initialize(ComPtr<ID3D11Device> device);

	bool Render(ComPtr<ID3D11DeviceContext> context, const RenderCall& call);

protected:
	// Members held by the shader. DirectX 11 specific, but also similar in OpenGL
	ComPtr<ID3D11VertexShader> vertexShader_;
	ComPtr<ID3D11PixelShader> pixelShader_;
	ComPtr<ID3D11InputLayout> inputLayout_;

	// D3D11 constant buffers. These buffers are where shader global variables are held,
	//  like the model/view/proj transformations. Since forming a new transfer to the
	//  graphics card is relatively expensive, it's good to group this data based on
	//  frequency of update and location.
	// In this case, the vertex shader requires information...
	//  - Per object: Model transformation
	//  - Per frame: View transformation, projection transformation (proj could also be per scene)
	// The fragment shader requires...
	//  - Per object: Material
	//  - Per frame: Camera position
	//  - Per scene: Sunlight position
	ComPtr<ID3D11Buffer> vsc_object_;
	ComPtr<ID3D11Buffer> vsc_frame_;
	ComPtr<ID3D11Buffer> psc_object_;
	ComPtr<ID3D11Buffer> psc_frame_;
	ComPtr<ID3D11Buffer> psc_scene_;

protected:
	// DirectX 11 specific optimizations. Similar ones could be used in OpenGL as well.
	//  I don't actually know Vulkan well enough to comment on if it could be used there
	struct // Constant buffer for the vertex shader, per object
	{
		struct VSC_PerObject_Type
		{
			Matrix Model;
		} VSC_PerObject;
		bool isDirty;
	} DVSC_PerObject;

	struct // Constant buffer for the vertex shader, per frame
	{
		struct VSC_PerFrame_Type
		{
			Matrix View;
			Matrix Proj;
		} VSC_PerFrame;
		bool isDirty;
	} DVSC_PerFrame;

	struct // Constant buffer for the pixel shader, per object
	{
		struct PSC_PerObject_Type
		{
			Material ObjectMaterial;
		} PSC_PerObject;
		bool isDirty;
	} DPSC_PerObject;

	struct // Constant buffer for the pixel shader, per frame
	{
		struct PSC_PerFrame_Type
		{
			Vec3 CameraPosition;
			float __extra;
		} PSC_PerFrame;
		bool isDirty;
	} DPSC_PerFrame;

	struct // Constant buffer for the pixel shader, per scene
	{
		struct PSC_PerScene_Type
		{
			DirectionalLight SunLight;
		} PSC_PerScene;
		bool isDirty;
	} DPSC_PerScene;
};

};