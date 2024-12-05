#pragma once
#include "math.h"
#include "float2.h"

/// <summary>
/// Column vector.
/// </summary>
struct float3
{
	float x, y, z;

	constexpr float3() : x(), y(), z() {};
	constexpr float3(float xyz) : x(xyz), y(xyz), z(xyz) {};
	constexpr float3(float x, float y, float z) : x(x), y(y), z(z) {}
	constexpr float3(const float2& xy, float z) : x(xy.x), y(xy.y), z(z) {}
	constexpr float3(float x, const float2& yz) : x(x), y(yz.x), z(yz.y) {}
	operator float* () const {
		return (float*)(&x);
	}

	constexpr float3(const float2& r) : x(r.x), y(r.y), z() {}
	constexpr explicit operator float2() { return float2(x, y); }

	constexpr float3 cross(const float3& r) const {
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
		return float3(y * r.z - z * r.y
					, z * r.x - x * r.z
					, x * r.y - y * r.x);
	}
	constexpr float dot(const float3& r) const {
		/* 
		 * A = [a, b, c]
		 * B = [d, e, f]
		 * dot(A, B) = ad + be + cf
		 */
		return x * r.x + y * r.y + z * r.z;
	}
	constexpr float lengthSq() const {
		return dot(*this);
	}
	constexpr float length() const {
		return math::sqrt(lengthSq());
	}
	constexpr float3 normalized() const {
		return *this / length();
	}

	constexpr float3 operator-() const {
		return float3(-x, -y, -z);
	}
	constexpr float3 operator*(const float3& r) const {
		return float3(x * r.x, y * r.y, z * r.z);
	}
	constexpr float3 operator*(const float r) const {
		return float3(x * r, y * r, z * r);
	}
	constexpr float3 operator/(const float3& r) const {
		return float3(x / r.x, y / r.y, z / r.z);
	}
	constexpr float3 operator/(const float r) const {
		return float3(x / r, y / r, z / r);
	}
	constexpr float3 operator+(const float3& r) const {
		return float3(x + r.x, y + r.y, z + r.z);
	}
	constexpr float3 operator+(const float r) const {
		return float3(x + r, y + r, z + r);
	}
	constexpr float3 operator-(const float3& r) const {
		return float3(x - r.x, y - r.y, z - r.z);
	}
	constexpr float3 operator-(const float r) const {
		return float3(x - r, y - r, z - r);
	}
	constexpr bool operator==(const float3& r) const
	{
		return x == r.x && y == r.y && z == r.z;
	}

	friend std::ostream& operator<<(std::ostream& stream, const float3& r) {
		stream << r.x << "," << r.y << "," << r.z;
		return stream;
	}
};

template<>
constexpr float3 math::min(const float3& l, const float3& r) {
	return float3(math::min(l.x, r.x), math::min(l.y, r.y), math::min(l.z, r.z));
}
template<>
constexpr float3 math::max(const float3& l, const float3& r) {
	return float3(math::max(l.x, r.x), math::max(l.y, r.y), math::max(l.z, r.z));
}
template<>
constexpr float3 math::clamp(const float3& val, const float3& min, const float3& max) {
	return float3(math::clamp(val.x, min.x, max.x), math::clamp(val.y, min.y, max.y), math::clamp(val.z, min.z, max.z));
}

template<>
constexpr float3 math::random<float3>() {
	return float3(math::random<float>(), math::random<float>(), math::random<float>());
}
template<>
constexpr float3 math::random<float3>(const float3& max) {
	return float3(math::random(max.x), math::random(max.y), math::random(max.z));
}
template<>
constexpr float3 math::random<float3>(const float3& min, const float3& max) {
	return float3(math::random(min.x, max.x), math::random(min.y, max.y), math::random(min.z, max.z));
}
