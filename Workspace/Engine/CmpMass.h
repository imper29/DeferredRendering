#pragma once
struct CmpMass {
	float value;

	/// <summary>
	/// I've made the mass default to one like any sane person would. :P
	/// </summary>
	CmpMass() : value(1.0f) {}
	CmpMass(float value) : value(value) {}
};
