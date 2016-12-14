#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <future>
#include <vector>
#include <MathExtras.h>

using Microsoft::WRL::ComPtr;

// This class interfaces between the logical code shown in the tutorials on YouTube, and
//  the Direct3D 11 code that I'm actually using to draw things.
// For more details, see the MaterialOnlyShader class - it is much more thoroughly commented.

namespace sess
{

class TexturedShader
{
	class Texture; // Definition is below - I want to keep this file organized
	               //  (1) graphics API agnostic, (2) implementation specific to D3D11,
	               //  (3) architecture and implementation specific to D3D11
	               // This class belongs in (2), but it is used by a call in (1)

public:
	// Data structures used by this shader (GRAPIHCS API AGNOSTIC)
	struct Vertex
	{
		Vec3 Position;
		float __one;
		Vec3 Normal;
		float __zero;
		float U; // These could be put in a Vec2 (and will be in the shader itself), but it would
		float V; //  only be for grouping. I'll just leave them separate like this.

	public:
		Vertex(const Vec3& pos, const Vec3& norm, float u, float v)
			: Position(pos), __one(1.f), Normal(norm), __zero(0.f), U(u), V(v)
		{}
	};

	// Material color will be multiplied with texture color for ambient and diffuse color, specular
	//  color will be applied as is. There are other approaches that could be taken, but I think this
	//  one finds a good balance between accurate and cheap.
	// This tutorial is about loading/using textures, not about advanced rendering techniques :-)
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

	// Calls used by this shader to set shader variables (would be the same in OGL/VK)
	//  These are not included in the render call itself, because they could change less frequently
	//  than render calls are made. The render call itself only stores geometry information.
	void SetModelTransform(const Matrix& modelTransform);
	void SetViewTransform(const Matrix& viewTransform);
	void SetProjectionTransform(const Matrix& projTransform);
	void SetObjectMaterial(const Material& objectMaterial);
	void SetCameraPosition(const Vec3& cameraPosition);
	void SetSunLight(const DirectionalLight& light);
	void SetTexture(const Texture& texture); // Definition of texture below
	
public:
	// Bridging stuff. The implementations of these are graphics AIP dependent, but the overall
	//  structure would be very similar in OGL/VK
	
	// Input of the render function - contains D3D11 buffers storing geometry data
	class RenderCall
	{
	public:
		RenderCall(ComPtr<ID3D11Device> device, const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);

	public:
		ComPtr<ID3D11Buffer> VertexBuffer;
		ComPtr<ID3D11Buffer> IndexBuffer;
		std::uint32_t NumberOfIndices;
	};

	// Wrapper around D3D11 texture
	class Texture
	{
	public:
		Texture(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context, const std::vector<unsigned char>& rawData, std::uint32_t width, std::uint32_t height);
		Texture(const Texture&) = default;
		~Texture() = default;

		ComPtr<ID3D11Texture2D> Buffer;
		ComPtr<ID3D11ShaderResourceView> SRV;
	};

public:
	// D3D11 specific stuff - OpenGL/Vulkan have similar concepts, but the organization would be very different
	TexturedShader();
	TexturedShader(const TexturedShader&) = delete;
	~TexturedShader() = default;

	std::future<bool> Initialize(ComPtr<ID3D11Device> device);

	bool Render(ComPtr<ID3D11DeviceContext> context, const RenderCall& call);

protected:
	// Members held by the shader. DirectX 11 specific, but also similar in OpenGL
	ComPtr<ID3D11VertexShader> vertexShader_;
	ComPtr<ID3D11PixelShader> pixelShader_;
	ComPtr<ID3D11InputLayout> inputLayout_;

	// D3D11 constant buffers
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

	ComPtr<ID3D11ShaderResourceView> boundSRV;
};

};