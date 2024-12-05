#pragma once
#include "ShapeType.h"
#include "ShapeAabb.h"
#include "ShapeCube.h"
#include "ShapePlane.h"
#include "ShapeSphere.h"

struct CmpCollider {
	union {
		ShapeAabb aabb;
		ShapeCube cube;
		ShapePlane plane;
		ShapeSphere sphere;
	};
	ShapeType type;

	CmpCollider(const ShapeAabb& aabb) : aabb(aabb), type(ShapeType::aabb) {}
	CmpCollider(const ShapeCube& cube) : cube(cube), type(ShapeType::cube) {}
	CmpCollider(const ShapePlane& plane) : plane(plane), type(ShapeType::plane) {}
	CmpCollider(const ShapeSphere& sphere) : sphere(sphere), type(ShapeType::sphere) {}
};
