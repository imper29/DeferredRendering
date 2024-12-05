#include "Bitmap.h"
#include <SDL_surface.h>
#include <SDL_image.h>

constexpr Color32 c0 = Color32(static_cast<uint8_t>(0), static_cast<uint8_t>(0), static_cast<uint8_t>(255), static_cast<uint8_t>(255));
constexpr Color32 c1 = Color32(static_cast<uint8_t>(255), static_cast<uint8_t>(0), static_cast<uint8_t>(255), static_cast<uint8_t>(255));
constexpr Color32 c2 = Color32(static_cast<uint8_t>(255), static_cast<uint8_t>(255), static_cast<uint8_t>(255), static_cast<uint8_t>(255));
constexpr Color32 c3 = Color32(static_cast<uint8_t>(0), static_cast<uint8_t>(255), static_cast<uint8_t>(255), static_cast<uint8_t>(255));

Bitmap::Bitmap() {
	width = 2;
	height = 2;
	pixels = new Color32[4];
	pixels[0] = c0;
	pixels[1] = c1;
	pixels[2] = c2;
	pixels[3] = c3;
}
Bitmap::Bitmap(const char* filename) {
	auto surf = IMG_Load(filename);
	if (surf) {
		width = surf->w;
		height = surf->h;
		pixels = new Color32[surf->w * surf->h];
		auto surfFormatted = SDL_ConvertSurfaceFormat(surf, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA32, 0);
		memcpy(pixels, surfFormatted->pixels, sizeof(Color32) * surfFormatted->w * surfFormatted->h);
		SDL_FreeSurface(surfFormatted);
		SDL_FreeSurface(surf);
	}
	else {
		width = 2;
		height = 2;
		pixels = new Color32[4];
		pixels[0] = c0;
		pixels[1] = c1;
		pixels[2] = c2;
		pixels[3] = c3;
	}
}
Bitmap::Bitmap(const int32_t width_, const int32_t height_) {
	width = width_;
	height = height_;
	pixels = new Color32[width_ * height_];
}
Bitmap::~Bitmap() {
	delete[] pixels;
}

void Bitmap::evaluate(std::function<Color32(const int2& position)> function) {
	for (int32_t x = 0; x < width; ++x) {
		for (int32_t y = 0; y < height; ++y) {
			int2 off = int2(x, y);
			(*this)[off] = function(off);
		}
	}
}
void Bitmap::evaluate(int2 pos, int2 size, std::function<Color32(const int2& position)> function) {
	if (pos.x < 0 || pos.y < 0 || pos.x + size.x > width || pos.y + size.y > height)
		throw "Bitmap evaluation failed because area is not inside the bounds of the bitmap.";
	for (int32_t x = 0; x < size.x; ++x) {
		for (int32_t y = 0; y < size.y; ++y) {
			int2 off = int2(x, y);
			(*this)[pos + off] = function(pos + off);
		}
	}
}

void Bitmap::clear(Color32 color, ColorSwizzle swizzle, ColorChannelMask mask) {
	int32_t length = width * height;
	for (uint8_t channel = 0; channel < 4; ++channel) {
		if ((uint8_t)mask & (1 << channel)) {
			ColorChannel channelIn = ColorChannel(channel);
			ColorChannel channelOut = swizzle.getChannel(channelIn);
			uint8_t value = color[channelIn];
			for (int32_t index = 0; index < length; ++index) {
				pixels[index][channelOut] = value;
			}
		}
	}
}
void Bitmap::clear(int2 pos, int2 size, Color32 color, ColorSwizzle swizzle, ColorChannelMask mask) {
	if (pos.x < 0 || pos.y < 0 || pos.x + size.x > width || pos.y + size.y > height)
		throw "Bitmap clear failed because area is not inside the bounds of the bitmap.";
	for (uint8_t channel = 0; channel < 4; ++channel) {
		if ((uint8_t)mask & (1 << channel)) {
			ColorChannel channelIn = ColorChannel(channel);
			ColorChannel channelOut = swizzle.getChannel(channelIn);
			for (int32_t x = 0; x < size.x; ++x) {
				for (int32_t y = 0; y < size.y; ++y) {
					int2 off = int2(x, y);
					(*this)[pos + off] = color;
				}
			}
		}
	}
}
void Bitmap::copyTo(Bitmap& dst, ColorSwizzle swizzle, ColorChannelMask mask) const
{
	if (width != dst.width)
		throw "Bitmap copy failed because source and destination bitmaps have different widths.";
	if (height != dst.height)
		throw "Bitmap copy failed because source and destination bitmaps have different heights.";
	int32_t length = width * height;
	for (uint8_t channel = 0; channel < 4; ++channel) {
		if ((uint8_t)mask & (1 << channel)) {
			ColorChannel channelIn = ColorChannel(channel);
			ColorChannel channelOut = swizzle.getChannel(channelIn);
			for (int32_t index = 0; index < length; ++index) {
				dst[index][channelOut] = pixels[index][channelIn];
			}
		}
	}
}
void Bitmap::copyTo(int2 srcPos, int2 dstPos, int2 size, Bitmap& dst, ColorSwizzle swizzle, ColorChannelMask mask) const {
	if (srcPos.x < 0 || srcPos.y < 0 || srcPos.x + size.x > width || srcPos.y + size.y > height)
		throw "Bitmap copy failed because source area is not inside the bounds of the source bitmap.";
	if (dstPos.x < 0 || dstPos.y < 0 || dstPos.x + size.x > width || dstPos.y + size.y > height)
		throw "Bitmap copy failed because destination area is not inside the bounds of the destination bitmap.";
	for (uint8_t channel = 0; channel < 4; ++channel) {
		if ((uint8_t)mask & (1 << channel)) {
			ColorChannel channelIn = ColorChannel(channel);
			ColorChannel channelOut = swizzle.getChannel(channelIn);
			for (int32_t x = 0; x < size.x; ++x) {
				for (int32_t y = 0; y < size.y; ++y) {
					int2 off = int2(x, y);
					dst[dstPos + off][channelOut] = (*this)[srcPos + off][channelIn];
				}
			}
		}
	}
}

Color32& Bitmap::operator[](int32_t index) {
	return pixels[index];
}
const Color32& Bitmap::operator[](int32_t index) const {
	return pixels[index];
}
Color32& Bitmap::operator[](int2 index) {
	return pixels[width * index.y + index.x];
}
const Color32& Bitmap::operator[](int2 index) const {
	return pixels[width * index.y + index.x];
}
