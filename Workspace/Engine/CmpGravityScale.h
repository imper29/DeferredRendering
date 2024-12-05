#pragma once
/// <summary>
/// Represents the scale by which entities are effected by gravity.
/// </summary>
struct CmpGravityScale
{
	float value;

	CmpGravityScale() : value(1.0f) {}
	CmpGravityScale(float value) : value(value) {}
};
