#pragma once

#include <cstdint>

// Utility class for dealing with colors
// Simplified down version of what I've taken to using in demos/games - only includes
//  methods that are used in this tutorial series

namespace sess
{

class Color
{
public:
	Color(float r, float g, float b, float a);
	Color(const Color&) = default;
	~Color() = default;

	// Two reasons for using std::uint8_t instead of the alternative (unsigned char):
	//  (1) Avoid association (especially with beginners) to strings
	//  (2) More clear that I'm using the type for number of bits instead of other properties
	static Color fromHex(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

	void packAsFloatArray(float* out) const;

	// Scale the color by a given factor, but clamp at 1.0 max and 0.0 min
	Color clampAndScale(float factor) const;

	Color withAlpha(float alpha) const;

	// Color palette: I just messed with values on https://coolors.co
	//  until I found https://coolors.co/151515-4b0082-a63d40-402e2a-e9b872
	// Why not? I'm no graphic designer, but I can stick with those colors
	// So soft and kind... Plus, with a channel name like "Indigo CS," I should
	//  probably have one of them be an indigo color.
public:
	static struct Palette
	{
		const static Color Black;
		const static Color Indigo;
		const static Color Red;
		const static Color Brown;
		const static Color CreamIGuess; // https://blog.xkcd.com/2010/05/03/color-survey-results/
		const static Color PureWhite;
	} Palette;

protected:
	float _r, _g, _b, _a;
};

};