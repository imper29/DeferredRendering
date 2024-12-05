#pragma once
struct CmpCameraFreecam {
	float speedLinear;
	float speedAngular;
public:
	CmpCameraFreecam() : speedLinear(25.0f), speedAngular(15.0f) {

	}
	CmpCameraFreecam(float speedLinear, float speedAngular) : speedLinear(speedLinear), speedAngular(speedAngular) {

	}
};
