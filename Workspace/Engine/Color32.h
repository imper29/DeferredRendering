#pragma once
#include <memory>
#include "float4.h"

enum class ColorChannelMask : uint8_t {
	R = 1 << 0,
	G = 1 << 1,
	B = 1 << 2,
	A = 1 << 3,

	RG = R | G,
	RGB = R | G | B,
	RGBA = R | G | B | A,
};
constexpr ColorChannelMask operator ~ (ColorChannelMask l) {
	return (ColorChannelMask)~(uint8_t)l;
}
constexpr ColorChannelMask operator ^ (ColorChannelMask l, ColorChannelMask r) {
	return (ColorChannelMask)((uint8_t)l ^ (uint8_t)r);
}
constexpr ColorChannelMask operator & (ColorChannelMask l, ColorChannelMask r) {
	return (ColorChannelMask)((uint8_t)l & (uint8_t)r);
}
constexpr ColorChannelMask operator | (ColorChannelMask l, ColorChannelMask r) {
	return (ColorChannelMask)((uint8_t)l | (uint8_t)r);
}

enum class ColorChannel : uint8_t {
	R = 0,
	G = 1,
	B = 2,
	A = 3,
};
union Color32
{
	uint32_t value;
	uint8_t rgba[4];
	struct { uint8_t r, g, b, a; };

	constexpr Color32() : r(), g(), b(), a() {}
	constexpr Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}
	constexpr Color32(float r, float g, float b, float a) : r((uint8_t)(r * 255.0f)), g((uint8_t)(g * 255.0f)), b((uint8_t)(b * 255.0f)), a((uint8_t)(a * 255.0f)) {}
	constexpr Color32(const float4& rgba) : r((uint8_t)(rgba.x * 255.0f)), g((uint8_t)(rgba.y * 255.0f)), b((uint8_t)(rgba.z * 255.0f)), a((uint8_t)(rgba.w * 255.0f)) {}

	constexpr uint8_t& operator[] (ColorChannel channel) {
		return rgba[(uint8_t)channel];
	}
	constexpr const uint8_t& operator[] (ColorChannel channel) const {
		return rgba[(uint8_t)channel];
	}

	constexpr operator float4 () {
		return float4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}
};
struct ColorSwizzle {
public:
	static constexpr uint8_t RRRR = 0b00000000;
	static constexpr uint8_t GGGG = 0b01010101;
	static constexpr uint8_t BBBB = 0b10101010;
	static constexpr uint8_t AAAA = 0b11111111;
	static constexpr uint8_t RGBA = 0b11100100;

	constexpr ColorSwizzle(uint8_t value) {
		m_Value = value;
	}
	constexpr ColorSwizzle(ColorChannel r, ColorChannel g, ColorChannel b, ColorChannel a) {
		m_Value = ((uint8_t)r << 0) | ((uint8_t)g << 2) | ((uint8_t)b << 4) | ((uint8_t)a << 6);
	}

	constexpr ColorChannel getChannel(ColorChannel inChannel) const {
		uint8_t shift = (uint8_t)inChannel * 2;
		uint8_t value = m_Value >> shift;
		return ColorChannel(value & 3);
	}
	constexpr void setChannel(ColorChannel inChannel, ColorChannel outChannel) {
		uint8_t shift = (uint8_t)inChannel * 2;
		uint8_t value = m_Value & ~(3 << shift);
		m_Value = value | ((uint8_t)outChannel << shift);
	}
private:
	uint8_t m_Value;
};
