#pragma once
#include "math.h"
#include "int3.h"

/// <summary>
/// Column vector.
/// </summary>
struct int4
{
	int x, y, z, w;

	constexpr int4() : x(), y(), z(), w() {}
	constexpr int4(int xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {};
	constexpr int4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
	constexpr int4(const int2& xy, int z, int w) : x(xy.x), y(xy.y), z(z), w(w) {}
	constexpr int4(int x, const int2& yz, int w) : x(x), y(yz.x), z(yz.y), w(w) {}
	constexpr int4(int x, int y, const int2& zw) : x(x), y(y), z(zw.x), w(zw.y) {}
	constexpr int4(const int2& xy, const int2& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
	constexpr int4(const int3& xyz, int w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
	constexpr int4(int x, const int3& yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
	operator int3 () const {
		return int3(x, y, z);
	}
	operator int* () const {
		return (int*)(&x);
	}

	constexpr int4(const int3& r) : x(r.x), y(r.y), z(r.z), w() {}
	constexpr explicit operator int3() { return int3(x, y, z); }
	constexpr int4(const int2& r) : x(r.x), y(r.y), z(), w() {}
	constexpr explicit operator int2() { return int2(x, y); }

	constexpr int dot(const int4& r) const {
		return x * r.x + y * r.y + z * r.z + w * r.w;
	}
	constexpr int lengthSq() const {
		return dot(*this);
	}

	constexpr int4 operator-() const {
		return int4(-x, -y, -z, -w);
	}
	constexpr int4 operator*(const int4& r) const {
		return int4(x * r.x, y * r.y, z * r.z, w * r.w);
	}
	constexpr int4 operator*(const int& r) const {
		return int4(x * r, y * r, z * r, w * r);
	}
	constexpr int4 operator/(const int4& r) const {
		return int4(x / r.x, y / r.y, z / r.z, w / r.w);
	}
	constexpr int4 operator/(const int& r) const {
		return int4(x / r, y / r, z / r, w / r);
	}
	constexpr int4 operator+(const int4& r) const {
		return int4(x + r.x, y + r.y, z + r.z, w + r.w);
	}
	constexpr int4 operator+(const int& r) const {
		return int4(x + r, y + r, z + r, w + r);
	}
	constexpr int4 operator-(const int4& r) const {
		return int4(x - r.x, y - r.y, z - r.z, w - r.w);
	}
	constexpr int4 operator-(const int& r) const {
		return int4(x - r, y - r, z - r, w - r);
	}

	constexpr bool operator==(const int4& r) const
	{
		return x == r.x && y == r.y && z == r.z && w == r.w;
	}

	friend std::ostream& operator<<(std::ostream& stream, const int4& r) {
		stream << r.x << "," << r.y << "," << r.z << "," << r.w;
		return stream;
	}
};

template<>
constexpr int4 math::min(const int4& l, const int4& r) {
	return int4(math::min(l.x, r.x), math::min(l.y, r.y), math::min(l.z, r.z), math::min(l.w, r.w));
}
template<>
constexpr int4 math::max(const int4& l, const int4& r) {
	return int4(math::max(l.x, r.x), math::max(l.y, r.y), math::max(l.z, r.z), math::max(l.w, r.w));
}
template<>
constexpr int4 math::clamp(const int4& val, const int4& min, const int4& max) {
	return int4(math::clamp(val.x, min.x, max.x), math::clamp(val.y, min.y, max.y), math::clamp(val.z, min.z, max.z), math::clamp(val.w, min.w, max.w));
}

template<>
constexpr int4 math::random<int4>() {
	return int4(math::random<int>(), math::random<int>(), math::random<int>(), math::random<int>());
}
template<>
constexpr int4 math::random<int4>(const int4& max) {
	return int4(math::random(max.x), math::random(max.y), math::random(max.z), math::random(max.w));
}
template<>
constexpr int4 math::random<int4>(const int4& min, const int4& max) {
	return int4(math::random(min.x, max.x), math::random(min.y, max.y), math::random(min.z, max.z), math::random(min.w, max.w));
}
