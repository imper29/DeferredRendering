#pragma once
#include "math.h"
#include "int2.h"

/// <summary>
/// Column vector.
/// </summary>
struct int3
{
	int x, y, z;

	constexpr int3() : x(), y(), z() {};
	constexpr int3(int xyz) : x(xyz), y(xyz), z(xyz) {};
	constexpr int3(int x, int y, int z) : x(x), y(y), z(z) {}
	constexpr int3(const int2& xy, int z) : x(xy.x), y(xy.y), z(z) {}
	constexpr int3(int x, const int2& yz) : x(x), y(yz.x), z(yz.y) {}
	operator int* () const {
		return (int*)(&x);
	}

	constexpr int3(const int2& r) : x(r.x), y(r.y), z() {}
	constexpr explicit operator int2() { return int2(x, y); }

	constexpr int3 cross(const int3& r) const {
		/*
		 * A = [a, b, c]
		 * B = [d, e, f]
		 *
		 * The cross product is just multiplication using anticommutation.
		 * ii = jj = kk = 0
		 * ij = k, jk = i, ki = j
		 * ji = -k, kj = -i, ik = -j
		 *
		 * //Derive cross product using anticommutation.
		 * cross(A, B) = (ai + bj + ck) * (di + ej + fk)
		 *			   = (adii + aeij + afik) + (bdji + dejj + bfjk) + (cdki + cekj + cfkk)
		 *			   = (+0 + aek - afj) + (-bdk + 0 + bfi) + (cdj - cei + 0)
		 *			   = (bfi - cei) + (cdj - afj) + (aek - bdk)
		 */
		return int3(y * r.z - z * r.y
			, z * r.x - x * r.z
			, x * r.y - y * r.x);
	}
	constexpr int dot(const int3& r) const {
		/*
		 * A = [a, b, c]
		 * B = [d, e, f]
		 * dot(A, B) = ad + be + cf
		 */
		return x * r.x + y * r.y + z * r.z;
	}
	constexpr int lengthSq() const {
		return dot(*this);
	}

	constexpr int3 operator-() const {
		return int3(-x, -y, -z);
	}
	constexpr int3 operator*(const int3& r) const {
		return int3(x * r.x, y * r.y, z * r.z);
	}
	constexpr int3 operator*(const int r) const {
		return int3(x * r, y * r, z * r);
	}
	constexpr int3 operator/(const int3& r) const {
		return int3(x / r.x, y / r.y, z / r.z);
	}
	constexpr int3 operator/(const int r) const {
		return int3(x / r, y / r, z / r);
	}
	constexpr int3 operator+(const int3& r) const {
		return int3(x + r.x, y + r.y, z + r.z);
	}
	constexpr int3 operator+(const int r) const {
		return int3(x + r, y + r, z + r);
	}
	constexpr int3 operator-(const int3& r) const {
		return int3(x - r.x, y - r.y, z - r.z);
	}
	constexpr int3 operator-(const int r) const {
		return int3(x - r, y - r, z - r);
	}
	constexpr bool operator==(const int3& r) const
	{
		return x == r.x && y == r.y && z == r.z;
	}

	friend std::ostream& operator<<(std::ostream& stream, const int3& r) {
		stream << r.x << "," << r.y << "," << r.z;
		return stream;
	}
};

template<>
constexpr int3 math::min(const int3& l, const int3& r) {
	return int3(math::min(l.x, r.x), math::min(l.y, r.y), math::min(l.z, r.z));
}
template<>
constexpr int3 math::max(const int3& l, const int3& r) {
	return int3(math::max(l.x, r.x), math::max(l.y, r.y), math::max(l.z, r.z));
}
template<>
constexpr int3 math::clamp(const int3& val, const int3& min, const int3& max) {
	return int3(math::clamp(val.x, min.x, max.x), math::clamp(val.y, min.y, max.y), math::clamp(val.z, min.z, max.z));
}

template<>
constexpr int3 math::random<int3>() {
	return int3(math::random<int>(), math::random<int>(), math::random<int>());
}
template<>
constexpr int3 math::random<int3>(const int3& max) {
	return int3(math::random(max.x), math::random(max.y), math::random(max.z));
}
template<>
constexpr int3 math::random<int3>(const int3& min, const int3& max) {
	return int3(math::random(min.x, max.x), math::random(min.y, max.y), math::random(min.z, max.z));
}
