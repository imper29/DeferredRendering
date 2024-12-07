#pragma once
#include "float4x4.h"
#include <iostream>
struct frustumPlanes {
	float4 planes[6];

	frustumPlanes() {

	};
	frustumPlanes(const float4x4& matrix) {
		//left
		planes[0].x = matrix.x.w + matrix.x.x;
		planes[0].y = matrix.y.w + matrix.y.x;
		planes[0].z = matrix.z.w + matrix.z.x;
		planes[0].w = matrix.w.w + matrix.w.x;
		//right
		planes[1].x = matrix.x.w - matrix.x.x;
		planes[1].y = matrix.y.w - matrix.y.x;
		planes[1].z = matrix.z.w - matrix.z.x;
		planes[1].w = matrix.w.w - matrix.w.x;
		//top
		planes[2].x = matrix.x.w - matrix.x.y;
		planes[2].y = matrix.y.w - matrix.y.y;
		planes[2].z = matrix.z.w - matrix.z.y;
		planes[2].w = matrix.w.w - matrix.w.y;
		//bottom
		planes[3].x = matrix.x.w + matrix.x.y;
		planes[3].y = matrix.y.w + matrix.y.y;
		planes[3].z = matrix.z.w + matrix.z.y;
		planes[3].w = matrix.w.w + matrix.w.y;
		//near
		planes[4].x = matrix.x.w + matrix.x.z;
		planes[4].y = matrix.y.w + matrix.y.z;
		planes[4].z = matrix.z.w + matrix.z.z;
		planes[4].w = matrix.w.w + matrix.w.z;
		//far
		planes[5].x = matrix.x.w - matrix.x.z;
		planes[5].y = matrix.y.w - matrix.y.z;
		planes[5].z = matrix.z.w - matrix.z.z;
		planes[5].w = matrix.w.w - matrix.w.z;

		//normalize
		for (size_t i = 0; i < 6; ++i) {
			planes[i] = planes[i] / float3(planes[i].x, planes[i].y, planes[i].z).length();
		}
	}

	bool contains(const float3& position, float radius) const {
		for (size_t i = 0; i < 6; ++i) {
			float d = float3(planes[i].x, planes[i].y, planes[i].z).dot(position) + planes[i].w;
			if (d < -radius)
				return false;
		}
		return true;
	}
};
