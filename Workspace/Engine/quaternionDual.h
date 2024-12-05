#pragma once
#include "math.h"
#include "float4.h"
#include "quaternion.h"
#include <iostream>
//INCOMPLETE BECAUSE I NEVER FULLLY IMPLEMENTED THIS!
struct quaternionDual
{
	quaternion v, e;

	constexpr quaternionDual() : v(), e() {};
	constexpr quaternionDual(const quaternion& v, const quaternion& e) : v(v), e(e) {}
	constexpr quaternionDual(float x, float y, float z, float w, float ex, float ey, float ez, float ew) : v(x, y, z, w), e(ex, ey, ez, ew) {}

	constexpr quaternionDual inverse() const {
		/*
		 * VARIABLES
		 * A^ = A + eB
		 * ALGEBRA
		 * A^^-1 = (A^-1)(1 - eB(A^-1))
		 * A^^-1 = (A^-1) - e((A^-1)B(A^-1))
		 */
		auto vInverse = v.inverse();
		return quaternionDual(vInverse, -(vInverse * e * vInverse));
	}
	constexpr quaternionDual conjugate() const {
		return quaternionDual(-v.x, -v.y, -v.z, v.w, -e.x, -e.y, -e.z, e.w);
	}
	constexpr quaternionDual conjugateDual() const {
		return quaternionDual(v.x, v.y, v.z, v.w, -e.x, -e.y, -e.z, -e.w);
	}
	constexpr quaternionDual conjugateDualQuaternion() const {
		return quaternionDual(-v.x, -v.y, -v.z, v.w, e.x, e.y, e.z, -e.w);
	}

	constexpr quaternionDual operator*(const float r) const {
		return quaternionDual(v * r, e * r);
	}
	constexpr quaternionDual operator/(const float r) const {
		return quaternionDual(v / r, e / r);
	}
	constexpr quaternionDual operator+(const quaternionDual& r) const {
		/*
		 * VARIABLES
		 * A^ = (A, B) = A + eB
		 * C^ = (C, D) = C + eD
		 *
		 * ALGEBRA
		 * A^+C^ = (A + eB)+(C + eD) = A + C + e(B + D)
		 */
		return quaternionDual(v + r.v, e + r.e);
	}
	constexpr quaternionDual operator-(const quaternionDual& r) const {
		/*
		 * VARIABLES
		 * A^ = (A, B) = A + eB
		 * C^ = (C, D) = C + eD
		 *
		 * ALGEBRA
		 * A^+C^ = (A - eB)+(C - eD) = A - C + e(B - D)
		 */
		return quaternionDual(v - r.v, e - r.e);
	}
	constexpr quaternionDual operator*(const quaternionDual& r) const {
		/*
		 * VARIABLES
		 * A^ = (A, B) = A + eB
		 * C^ = (C, D) = C + eD
		 * 
		 * ALGEBRA
		 * A^C^ = (A + eB)(C + eD) = AC + e(AD + BC)
		 */
		return quaternionDual(v * r.v, v * r.e + e * r.v);
	}
	constexpr float3 operator*(const float3& r) const {
		const quaternionDual t = *this * translation(r);
		return float3(t.e.x * 2.0f, t.e.y * 2.0f, t.e.z * 2.0f);
	}

	constexpr static quaternionDual axisAngle(const float3& axis, float angle) {
		return quaternionDual(quaternion::axisAngle(axis, angle), quaternion(0, 0, 0, 0));
	}
	constexpr static quaternionDual translation(const float3& translation) {
		return quaternionDual(quaternion(0, 0, 0, 1), quaternion(translation * 0.5f, 0));
	}
	constexpr static quaternionDual translationAxisAngle(const float3& translation, const float3& axis, float angle) {
		return quaternionDual(quaternion::axisAngle(axis, angle), quaternion(translation * 0.5f, 0));
	}
	constexpr static quaternionDual sclerp(const quaternionDual& a, const quaternionDual& b, float t) {
		/*
		 * https://arxiv.org/pdf/2303.13395.pdf
		 * 
		 * sclerp(a, b, t) = a * (pow(conj(a) * b, t))
		 */
		//THIS IS WRONG! VERY WRONG!
		return a * pow(a.conjugate() * b, t);
	}
	static quaternionDual pow(const quaternionDual& q, float t) {
		/*
		 * cos(a + eb) = cos(a) - eb sin(a)
		 * sin(a + eb) = sin(a) - eb cos(a)
		 * 
		 * https://arxiv.org/pdf/2303.13395.pdf
		 * q = a + B + ec + eD
		 * 
		 * r = 2acos(a)
		 * L = B / sin(r / 2)
		 * d = -c * 2 / sin(r / 2)
		 * M = D / sin(r / 2) - L * d / 2 * cos(r / 2) / sin(r / 2)
		 * 
		 * q^t = cos(r/2 + ed/2) + (L + eM)(sin(r/2 + ed/2))
		 * 
		 * q^t = cos(r/2) - ed*sin(r/2) + (L + eM)(sin(r/2) - ed*cos(r/2))
		 * 
		 * q^t = cos(r/2) - ed*sin(r/2) + (L*sin(r/2) - L*ed*cos(r/2) + eM*sin(r/2) - 0)
		 */
		//THIS IS WRONG! VERY WRONG!
		auto r = 2.0f * acos(q.v.w);
		auto L = float3(q.v.x, q.v.y, q.v.z) / sin(r / 2.0f);
		auto d = -q.e.w * 2.0f / sin(r / 2.0f);
		auto M = float3(q.e.x, q.e.y, q.e.z) / sin(r / 2.0f) - L * (d / 2.0f * cos(r / 2.0f) / sin(r / 2.0f));
		return quaternionDual(quaternion(L * sin(r / 2.0f), cos(r / 2.0f)), quaternion(M * sin(r / 2.0f) - L * (d * cos(r / 2.0f)), d * sin(r / 2.0f)));
	}
};
