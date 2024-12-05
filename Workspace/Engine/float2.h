#pragma once
#include "math.h"
#include <ostream>

/// <summary>
/// Column vector.
/// </summary>
struct float2
{
	float x, y;

	constexpr float2() : x(), y() {};
	constexpr float2(float xy) : x(xy), y(xy) {};
	constexpr float2(float x, float y) : x(x), y(y) {}
	operator float* () const {
		return (float*)(&x);
	}

	constexpr float dot(const float2& r) const {
		return x * r.x + y * r.y;
	}
	constexpr float lengthSq() const {
		return dot(*this);
	}
	constexpr float length() const {
		return math::sqrt(lengthSq());
	}
	constexpr float2 normalized() const {
		return *this / length();
	}

	constexpr float2 operator-() const {
		return float2(-x, -y);
	}
	constexpr float2 operator*(const float2& r) const {
		return float2(x * r.x, y * r.y);
	}
	constexpr float2 operator*(const float& r) const {
		return float2(x * r, y * r);
	}
	constexpr float2 operator/(const float2& r) const {
		return float2(x / r.x, y / r.y);
	}
	constexpr float2 operator/(const float& r) const {
		return float2(x / r, y / r);
	}
	constexpr float2 operator+(const float2& r) const {
		return float2(x + r.x, y + r.y);
	}
	constexpr float2 operator+(const float& r) const {
		return float2(x + r, y + r);
	}
	constexpr float2 operator-(const float2& r) const {
		return float2(x - r.x, y - r.y);
	}
	constexpr float2 operator-(const float& r) const {
		return float2(x - r, y - r);
	}

	constexpr bool operator==(const float2& r) const
	{
		return x == r.x && y == r.y;
	}

	friend std::ostream& operator<<(std::ostream& stream, const float2& r) {
		stream << r.x << "," << r.y;
		return stream;
	}
};

template<>
constexpr float2 math::min(const float2& l, const float2& r) {
	return float2(math::min(l.x, r.x), math::min(l.y, r.y));
}
template<>
constexpr float2 math::max(const float2& l, const float2& r) {
	return float2(math::max(l.x, r.x), math::max(l.y, r.y));
}
template<>
constexpr float2 math::clamp(const float2& val, const float2& min, const float2& max) {
	return float2(math::clamp(val.x, min.x, max.x), math::clamp(val.y, min.y, max.y));
}

template<>
constexpr float2 math::random<float2>() {
	return float2(math::random<float>(), math::random<float>());
}
template<>
constexpr float2 math::random<float2>(const float2& max) {
	return float2(math::random(max.x), math::random(max.y));
}
template<>
constexpr float2 math::random<float2>(const float2& min, const float2& max) {
	return float2(math::random(min.x, max.x), math::random(min.y, max.y));
}
