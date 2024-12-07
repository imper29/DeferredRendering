#pragma once
#include "float4.h"
#include "quaternion.h"

static float det(float xx, float yx, float zx, float xy, float yy, float zy, float xz, float yz, float zz) {
	//x.x, y.x, z.x
	//x.y, y.y, z.y
	//x.z, y.z, z.z
	return xx * (yy * zz - zy * yz)
		 - yx * (xy * zz - zy * xz)
		 + zx * (xy * yz - yy * xz);
}

/// <summary>
/// Column-major matrix.
/// </summary>
struct float4x4
{
	float4 x, y, z, w;

	constexpr float4x4() : x(), y(), z(), w() {}
	constexpr float4x4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) : x(m00, m10, m20, m30), y(m01, m11, m21, m31), z(m02, m12, m22, m32), w(m03, m13, m23, m33) {}
	constexpr float4x4(const float4& x, const float4& y, const float4& z, const float4& w) : x(x), y(y), z(z), w(w) {}
	operator float* () const {
		return x;
	}

	constexpr float3 getTranslation() const {
		return float3(w.x, w.y, w.z);
	}
	constexpr float4x4 withoutTranslation() const {
		return float4x4(x, y, z, float4(0, 0, 0, 1));
	}

	constexpr float4x4 transpose() const {
		return float4x4(float4(x.x, y.x, z.x, w.x),
						float4(x.y, y.y, z.y, w.y),
						float4(x.z, y.z, z.z, w.z),
						float4(x.w, y.w, z.w, w.w));
	}
	constexpr float4x4 inverse() const {
		float4x4 a = cofactor().transpose();
		float n = 1.0f / (x.x * a.x.x + y.x * a.x.y + z.x * a.x.z + w.x * a.x.w);
		a.x = float4(n * a.x.x, n * a.x.y, n * a.x.z, n * a.x.w);
		a.y = float4(n * a.y.x, n * a.y.y, n * a.y.z, n * a.y.w);
		a.z = float4(n * a.z.x, n * a.z.y, n * a.z.z, n * a.z.w);
		a.w = float4(n * a.w.x, n * a.w.y, n * a.w.z, n * a.w.w);
		return a;
	}
	constexpr float4x4 cofactor() const {
		//x.x, y.x, z.x, w.x
		//x.y, y.y, z.y, w.y
		//x.z, y.z, z.z, w.z
		//x.w, y.w, z.w, w.w
		float4x4 m = minor();
		m.y.x = -m.y.x;
		m.w.x = -m.w.x;
		m.x.y = -m.x.y;
		m.z.y = -m.z.y;
		m.y.z = -m.y.z;
		m.w.z = -m.w.z;
		m.x.w = -m.x.w;
		m.z.w = -m.z.w;
		return m;
	}
	constexpr float4x4 minor() const {
		//x.x, y.x, z.x, w.x
		//x.y, y.y, z.y, w.y
		//x.z, y.z, z.z, w.z
		//x.w, y.w, z.w, w.w
		return float4x4(det(y.y, z.y, w.y, y.z, z.z, w.z, y.w, z.w, w.w), det(x.y, z.y, w.y, x.z, z.z, w.z, x.w, z.w, w.w),
						det(x.y, y.y, w.y, x.z, y.z, w.z, x.w, y.w, w.w), det(x.y, y.y, z.y, x.z, y.z, z.z, x.w, y.w, z.w),

						det(y.x, z.x, w.x, y.z, z.z, w.z, y.w, z.w, w.w), det(x.x, z.x, w.x, x.z, z.z, w.z, x.w, z.w, w.w),
						det(x.x, y.x, w.x, x.z, y.z, w.z, x.w, y.w, w.w), det(x.x, y.x, z.x, x.z, y.z, z.z, x.w, y.w, z.w),

						det(y.x, z.x, w.x, y.y, z.y, w.y, y.w, z.w, w.w), det(x.x, z.x, w.x, x.y, z.y, w.y, x.w, z.w, w.w),
						det(x.x, y.x, w.x, x.y, y.y, w.y, x.w, y.w, w.w), det(x.x, y.w, z.x, x.y, y.y, z.y, x.w, y.w, z.w),

						det(y.x, z.x, w.x, y.y, z.y, w.y, y.z, z.z, w.z), det(x.x, z.x, w.x, x.y, z.y, w.y, x.z, z.z, w.z),
						det(x.x, y.x, w.x, x.y, y.y, w.y, x.z, y.z, w.z), det(x.x, y.x, z.x, x.y, y.y, z.y, x.z, y.z, z.z));
	}

	constexpr float4x4 operator*(const float4x4& r) const {
		auto t = transpose();
		return float4x4(t.x.dot(r.x), t.x.dot(r.y), t.x.dot(r.z), t.x.dot(r.w),
						t.y.dot(r.x), t.y.dot(r.y), t.y.dot(r.z), t.y.dot(r.w),
						t.z.dot(r.x), t.z.dot(r.y), t.z.dot(r.z), t.z.dot(r.w),
						t.w.dot(r.x), t.w.dot(r.y), t.w.dot(r.z), t.w.dot(r.w));
	}
	constexpr float4 operator*(const float4& r) const {
		return float4((x.x * r.x + y.x * r.y + z.x * r.z + w.x * r.w),
					  (x.y * r.x + y.y * r.y + z.y * r.z + w.y * r.w),
					  (x.z * r.x + y.z * r.y + z.z * r.z + w.z * r.w),
					  (x.w * r.x + y.w * r.y + z.w * r.z + w.w * r.w));
	}
	friend std::ostream& operator<<(std::ostream& stream, const float4x4& r) {
		float4x4 t = r.transpose();
		stream << t.x << std::endl << t.y << std::endl << t.z << std::endl << t.w;
		return stream;
	}

	static constexpr float4x4 identity() {
		return float4x4(float4(1, 0, 0, 0), float4(0, 1, 0, 0), float4(0, 0, 1, 0), float4(0, 0, 0, 1));
	}
	static constexpr float4x4 perspective(float aspectRatio, float fov, float near, float far) {
		float s = 1.0f / (math::tan(fov * 0.5f));
		float a = (near + far) / (near - far);
		float b = (2.0f * near * far) / (near - far);
		return float4x4(s / aspectRatio, 0, 0, 0,
						0, s, 0, 0,
						0, 0, a, -1,
						0, 0, b, 0);
	}
	static constexpr float4x4 orthographic(float aspectRatio, float height, float near, float far) {
		float halfHeight = height * 0.5f;
		float halfWidth = halfHeight * aspectRatio;
		return orthographic(float3(-halfWidth, -halfHeight, near), float3(halfWidth, halfHeight, far));
	}
	static constexpr float4x4 orthographic(const float3& min, const float3& max) {
		float3 sum = max + min;
		float3 dif = max - min;
		return float4x4(2.0f / dif.x, 0, 0, -sum.x / dif.x,
						0, 2.0f / dif.y, 0, -sum.y / dif.y,
						0, 0, -2.0f / dif.z, -sum.z / dif.z,
						0, 0, 0, 1);
	}
	
	static constexpr float4x4 rotate(const quaternion& rotation) {
		return float4x4(2.0f * (rotation.w * rotation.w + rotation.x * rotation.x) - 1.0f,
						2.0f * (rotation.x * rotation.y - rotation.w * rotation.z),
						2.0f * (rotation.x * rotation.z + rotation.w * rotation.y),
						0.0f,

						2.0f * (rotation.x * rotation.y + rotation.w * rotation.z),
						2.0f * (rotation.w * rotation.w + rotation.y * rotation.y) - 1.0f,
						2.0f * (rotation.y * rotation.z - rotation.w * rotation.x),
						0.0f,
			
						2.0f * (rotation.x * rotation.z - rotation.w * rotation.y),
						2.0f * (rotation.y * rotation.z + rotation.w * rotation.x),
						2.0f * (rotation.w * rotation.w + rotation.z * rotation.z) - 1.0f,
						0.0f,
			
						0.0f,
						0.0f,
						0.0f,
						1.0f);
	}
	static constexpr float4x4 translate(const float3& translation) {
		return float4x4(1, 0, 0, translation.x,
						0, 1, 0, translation.y,
						0, 0, 1, translation.z,
						0, 0, 0, 1);
	}
	static constexpr float4x4 translateRotate(const float3& translation, const quaternion& rotation) {
		return float4x4(2.0f * (rotation.w * rotation.w + rotation.x * rotation.x) - 1.0f,
			2.0f * (rotation.x * rotation.y - rotation.w * rotation.z),
			2.0f * (rotation.x * rotation.z + rotation.w * rotation.y),
			translation.x,

			2.0f * (rotation.x * rotation.y + rotation.w * rotation.z),
			2.0f * (rotation.w * rotation.w + rotation.y * rotation.y) - 1.0f,
			2.0f * (rotation.y * rotation.z - rotation.w * rotation.x),
			translation.y,

			2.0f * (rotation.x * rotation.z - rotation.w * rotation.y),
			2.0f * (rotation.y * rotation.z + rotation.w * rotation.x),
			2.0f * (rotation.w * rotation.w + rotation.z * rotation.z) - 1.0f,
			translation.z,

			0.0f,
			0.0f,
			0.0f,
			1.0f);
	}
	static constexpr float4x4 scale(const float3& scale) {
		return float4x4(scale.x, 0, 0, 0,
						0, scale.y, 0, 0,
						0, 0, scale.z, 0,
						0, 0, 0, 1);
	}
};
