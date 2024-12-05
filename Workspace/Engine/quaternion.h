#pragma once
#include "math.h"
#include "float4.h"
//UNTESTED
struct quaternion
{
	float x, y, z, w;

	constexpr quaternion() : x(), y(), z(), w() {};
	constexpr quaternion(const float4& xyzw) : x(xyzw.x), y(xyzw.y), z(xyzw.z), w(xyzw.w) {}
	constexpr quaternion(const float3& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
	constexpr quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	constexpr float lengthSq() const {
		return x * x + y * y + z * z + w * w;
	}
	constexpr float length() const {
		return math::sqrt(lengthSq());
	}
	constexpr quaternion inverse() const {
		/* 
		 * Q = (w + xi + yj + zk)
		 * Q^-1 = (w - xi - yj - zk) / ||Q||^2
		 */
		float lengthS = lengthSq();
		return quaternion(-x / lengthS, -y / lengthS, -z / lengthS, w / lengthS);
	}
	constexpr quaternion conjugate() const {
		return quaternion(-x, -y, -z, w);
	}
	constexpr quaternion normalized() const {
		float len = length();
		return quaternion(x / len, y / len, z / len, w / len);
	}

	//TODO: implement acos in math lib so this can be made into constexpr.
	static quaternion slerp(const quaternion& a, const quaternion& b, float t) {
		/*
		 * A = first quaternion.
		 * B = second quaternion.
		 * C = quaternion that rotates from A to B
		 *
		 * B = AC
		 * B(A^-1) = AC(A^-1)
		 * B(A^-1) = C
		 *
		 * C.w = cos(theta / 2) = A.w * B.w + A.x * B.x + A.y * B.y + A.z * B.z
		 * sin(theta / 2) = sqrt(1 - cos(theta / 2)^2)
		 * slerp(A, B) = (A * sin((1 - t) * theta / 2) + B * sin(t * theta / 2)) / sin(theta / 2)
		 */
		float cosHalfTheta = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		//There is no angle between quaternions.
		if (cosHalfTheta >= 1.0f)
			return a;
		float halfTheta = acos(cosHalfTheta);
		float sinHalfTheta = math::sqrt(1.0f - cosHalfTheta * cosHalfTheta);
		float ratioA = math::sin((1.0f - t) * halfTheta) / sinHalfTheta;
		float ratioB = math::sin(t * halfTheta) / sinHalfTheta;
		return a * ratioA + b * ratioB;
	}
	constexpr static quaternion identity() {
		return quaternion(float3(0, 0, 0), 1);
	}

	constexpr quaternion operator-() const {
		return quaternion(-x, -y, -z, -w);
	}
	constexpr quaternion operator+(const quaternion& r) const {
		return quaternion(x + r.x, y + r.y, z + r.z, w + r.w);
	}
	constexpr quaternion operator-(const quaternion& r) const {
		return quaternion(x - r.x, y - r.y, z - r.z, w - r.w);
	}
  	constexpr quaternion operator*(const quaternion& r) const {
		/*
		 * MULTIPLICATION PROPERTIES
		 * ii = jj = kk = -1
		 * ij = -ji = k
		 * jk = -kj = i
		 * ki = -ik = j
		 * 
		 * VARIABLES
		 * A = a + bi + cj + dk
		 * B = e + fi + gj + hk
		 * 
		 * ALGEBRA
		 * AB = (a + bi + cj + dk)(e + fi + gj + hk)
		 * AB = a(e + fi + gj + hk) + bi(e + fi + gj + hk) + cj(e + fi + gj + hk) + dk(e + fi + gj + hk)
		 * AB = ae + afi + agj + ahk + bei + bfii + bgij + bhik + cej + cfji + cgjj + chjk + dek + dfki + dgkj + dhkk
		 * AB = ae + afi + agj + ahk + bei - bf + bgk - bhj + cej - cfk - cg + chi + dek + dfj - dgi - dh
		 * AB = (ae - bf - cg - dh) + i(af + be + ch - dg) + j(ag - bh + ce + df) + k(ah + bg - cf + de)
		 * 
		 * CONVERT TO C++ VARIBLES
		 * a = w
		 * b = x
		 * c = y
		 * d = z
		 * e = r.w
		 * f = r.x
		 * g = r.y
		 * h = r.z
		 */
		return quaternion(w * r.x + x * r.w + y * r.z - z * r.y,
						  w * r.y - x * r.z + y * r.w + z * r.x,
						  w * r.z + x * r.y - y * r.x + z * r.w,
						  w * r.w - x * r.x - y * r.y - z * r.z);
	}
	constexpr quaternion operator*(const float r) const {
		return quaternion(x * r, y * r, z * r, w * r);
	}
	constexpr quaternion operator/(const float r) const {
		return quaternion(x / r, y / r, z / r, w / r);
	}
	constexpr float3 operator*(const float3& r) const {
		/* 
		 * VARIABLES
		 * A = u + s = (ai + bj + ck) + d
		 * B = v + 0 = (ei + fj + gk) + h
		 * 
		 * ALGEBRA
		 * ABA* = (u, s)(v, 0)(-u, s)
		 *		= (sv+u*v, -u.v)(-u, s)
		 *		= ((-u.v)(-u) + s(sv+u*v) + (sv+u*v)*(-u), discarded)
		 *		= ((-u.v)(-u) + ssv + s(u*v) + sv*(-u) + u*(u*v*), discarded)
		 *		= ((u.v)u + ssv + s(u*v) + s(u*v) + u*(u*v*), discarded)
		 *		= ((u.v)u + ssv + 2s(u*v) + u*(u*v*), discarded)
		 *		= ((u.v)u + ssv + 2s(u*v) + (u.v)u - (u.u)v, discarded)
		 *		= (2(u.v)u + ssv - (u.u)v + 2s(u*v), discarded)
		 *		= (2(u.v)u + (ss - u.u)v + 2s(u*v), discarded)
		 *		= (u2(u.v) + v(ss - u.u) + (u*v)2s, discarded)
		 * 
		 * CONVERT TO C++ VARIBLES
		 * u = xyz
		 * v = r
		 * s = w
		 */
		float3 xyz = float3(x, y, z);
		return xyz * (2.0f * xyz.dot(r))
			 + r * (w * w - xyz.lengthSq())
			 + xyz.cross(r) * (2.0f * w);
	}

	//TODO: implement acos in math lib so this can be made into constexpr.
	static quaternion lookAt(const float3& from, const float3& to, const float3& forward) {
		float3 forwardLook = (to - from).normalized();
		float dot = forward.dot(forwardLook);
		float rotAngle = acos(dot);
		float3 rotAxis = forward.cross(forwardLook).normalized();
		return fromAxisAngle(rotAxis, rotAngle);
	}
	constexpr static quaternion fromAxisAngle(const float3& axis, float angle) {
		float a = angle * 0.5f;
		float s = math::sin(a);
		float c = math::cos(a);
		return quaternion(axis * s, c);
	}
	constexpr static quaternion fromEulerAngle(const float3& euler) {
		float cr = math::cos(euler.x * 0.5f);
		float sr = math::sin(euler.x * 0.5f);
		float cp = math::cos(euler.y * 0.5f);
		float sp = math::sin(euler.y * 0.5f);
		float cy = math::cos(euler.z * 0.5f);
		float sy = math::sin(euler.z * 0.5f);

		quaternion q;
		q.w = cr * cp * cy + sr * sp * sy;
		q.x = sr * cp * cy - cr * sp * sy;
		q.y = cr * sp * cy + sr * cp * sy;
		q.z = cr * cp * sy - sr * sp * cy;

		return q;
	}
};
