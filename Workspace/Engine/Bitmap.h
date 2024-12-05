#pragma once
#include "int2.h"
#include "Color32.h"
#include <functional>

class Bitmap {
public:
	Bitmap();
	Bitmap(const char* filename);
	Bitmap(const int32_t width, const int32_t height);
	~Bitmap();

	void evaluate(std::function<Color32(const int2& position)> function);
	void evaluate(int2 pos, int2 size, std::function<Color32(const int2& position)> function);
	void clear(Color32 color, ColorSwizzle swizzle = ColorSwizzle::RGBA, ColorChannelMask mask = ColorChannelMask::RGBA);
	void clear(int2 pos, int2 size, Color32 color, ColorSwizzle swizzle = ColorSwizzle::RGBA, ColorChannelMask mask = ColorChannelMask::RGBA);
	void copyTo(Bitmap& dst, ColorSwizzle swizzle = ColorSwizzle::RGBA, ColorChannelMask mask = ColorChannelMask::RGBA) const;
	void copyTo(int2 srcPos, int2 dstPos, int2 size, Bitmap& dst, ColorSwizzle swizzle = ColorSwizzle::RGBA, ColorChannelMask mask = ColorChannelMask::RGBA) const;

	Color32& operator [](int32_t index);
	const Color32& operator [](int32_t index) const;
	Color32& operator [](int2 index);
	const Color32& operator [](int2 index) const;

	int32_t width;
	int32_t height;
	Color32* pixels;
};
