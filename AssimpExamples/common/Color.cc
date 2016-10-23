#include <Color.h>

#include <cmath>

namespace sess
{

// Color palette
const Color Color::Palette::Black = Color::fromHex(0x15u, 0x15u, 0x15u, 0xffu); // #151515
const Color Color::Palette::Indigo = Color::fromHex(0x4bu, 0x00u, 0x82u, 0xffu); // #fb0082
const Color Color::Palette::Red = Color::fromHex(0xa6u, 0x3du, 0x40u, 0xffu); // #a63d40
const Color Color::Palette::Brown = Color::fromHex(0x40u, 0x2eu, 0x2au, 0xffu); // #402e2a
const Color Color::Palette::CreamIGuess = Color::fromHex(0xf9u, 0xb8u, 0x72u, 0xffu); // #e9b872
const Color Color::Palette::PureWhite = Color::fromHex(0xffu, 0xffu, 0xffu, 0xffu);

Color::Color(float r, float g, float b, float a)
	: _r(r), _g(g), _b(b), _a(a)
{}

Color Color::fromHex(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
{
	return Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

// Take advantage of the packing of class data
void Color::packAsFloatArray(float* out) const
{
	out[0] = _r;
	out[1] = _g;
	out[2] = _b;
	out[3] = _a;
}

Color Color::clampAndScale(float factor) const
{
	return Color
	(
		fmaxf(fminf(_r * factor, 1.f), 0.f),
		fmaxf(fminf(_g * factor, 1.f), 0.f),
		fmaxf(fminf(_b * factor, 1.f), 0.f),
		fmaxf(fminf(_a * factor, 1.f), 0.f)
	);
}

Color Color::withAlpha(float alpha) const
{
	return Color
	(
		_r,
		_g,
		_b,
		alpha
	);
}

};