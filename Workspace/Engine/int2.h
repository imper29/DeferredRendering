#pragma once
#include "math.h"
#include <ostream>

/// <summary>
/// Column vector.
/// </summary>
struct int2
{
	int x, y;

	constexpr int2() : x(), y() {};
	constexpr int2(int xy) : x(xy), y(xy) {};
	constexpr int2(int x, int y) : x(x), y(y) {}
	operator int* () const {
		return (int*)(&x);
	}

	constexpr int dot(const int2& r) const {
		return x * r.x + y * r.y;
	}
	constexpr int lengthSq() const {
		return dot(*this);
	}

	constexpr int2 operator-() const {
		return int2(-x, -y);
	}
	constexpr int2 operator*(const int2& r) const {
		return int2(x * r.x, y * r.y);
	}
	constexpr int2 operator*(const int& r) const {
		return int2(x * r, y * r);
	}
	constexpr int2 operator/(const int2& r) const {
		return int2(x / r.x, y / r.y);
	}
	constexpr int2 operator/(const int& r) const {
		return int2(x / r, y / r);
	}
	constexpr int2 operator+(const int2& r) const {
		return int2(x + r.x, y + r.y);
	}
	constexpr int2 operator+(const int& r) const {
		return int2(x + r, y + r);
	}
	constexpr int2 operator-(const int2& r) const {
		return int2(x - r.x, y - r.y);
	}
	constexpr int2 operator-(const int& r) const {
		return int2(x - r, y - r);
	}

	constexpr bool operator==(const int2& r) const
	{
		return x == r.x && y == r.y;
	}

	friend std::ostream& operator<<(std::ostream& stream, const int2& r) {
		stream << r.x << "," << r.y;
		return stream;
	}
};

template<>
constexpr int2 math::min(const int2& l, const int2& r) {
	return int2(math::min(l.x, r.x), math::min(l.y, r.y));
}
template<>
constexpr int2 math::max(const int2& l, const int2& r) {
	return int2(math::max(l.x, r.x), math::max(l.y, r.y));
}
template<>
constexpr int2 math::clamp(const int2& val, const int2& min, const int2& max) {
	return int2(math::clamp(val.x, min.x, max.x), math::clamp(val.y, min.y, max.y));
}

template<>
constexpr int2 math::random<int2>() {
	return int2(math::random<int>(), math::random<int>());
}
template<>
constexpr int2 math::random<int2>(const int2& max) {
	return int2(math::random(max.x), math::random(max.y));
}
template<>
constexpr int2 math::random<int2>(const int2& min, const int2& max) {
	return int2(math::random(min.x, max.x), math::random(min.y, max.y));
}
