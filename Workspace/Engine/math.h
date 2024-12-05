#pragma once
#include <cmath>
#include <type_traits>
#include <limits>

struct math
{
private:
	template<typename T>
	static constexpr T COMPILE_sqrtNewtonRaphson(T x, T curr, T prev) noexcept {
		return curr == prev ? curr : COMPILE_sqrtNewtonRaphson(x, T(0.5) * (curr + x / curr), curr);
	}
	template<typename T>
	static constexpr T COMPILE_sinTaylorSeriesExpansion(T x2, int i, int k, T xn, long long nf) {
		return (i > 18) ? T(0) : (k * xn / nf) + COMPILE_sinTaylorSeriesExpansion(x2, i + 2, -k, xn * x2, nf * (i + 1) * (i + 2));
	}
	template<typename T>
	static constexpr T COMPILE_sinTaylorSeries(T x) {
		T xModPi2 = math::mod(x, math::PI<T>);
		if (xModPi2 < math::PI<T> / T(2))
			return -COMPILE_sinTaylorSeriesExpansion(xModPi2 * xModPi2, 1, 1, xModPi2, 1);
		return COMPILE_sinTaylorSeriesExpansion(xModPi2 * xModPi2, 1, 1, xModPi2, 1);
	}

public:
	template<typename T = float>
	static constexpr T E = T(2.7182818284590452353602874713527);
	template<typename T = float>
	static constexpr T PI = T(3.141592653589793238462643383279502884197);
	template<typename T = float>
	static constexpr T PI2 = PI<T> / T(2);
	template<typename T = float>
	static constexpr T PI4 = PI<T> / T(4);
	template<typename T = float>
	static constexpr T DegToRad = PI<T> / T(180);
	template<typename T = float>
	static constexpr T RadToDeg = T(180) / PI<T>;

	template<typename T>
	static constexpr T mod(const T& l, const T& r) {
		return l - (r * (long long)(l / r));
	}
	template<typename T>
	static constexpr T sqrt(const T& x) noexcept {
		if (std::is_constant_evaluated()) {
			if (x >= 0 && x < std::numeric_limits<T>::infinity())
				return COMPILE_sqrtNewtonRaphson(x, x, T(0));
			return std::numeric_limits<T>::quiet_NaN();
		}
		else {
			return std::sqrt(x);
		}
	}
	template<typename T>
	static constexpr T abs(const T& x) {
		if (x < T(0))
			return T(0);
		return x;
	}

	template<typename T>
	static constexpr T sin(const T& x) noexcept {
		if (std::is_constant_evaluated()) {
			return T(COMPILE_sinTaylorSeries<double>(x));
		}
		else {
			return std::sin(x);
		}
	}
	template<typename T>
	static constexpr T csc(const T& x) {
		return T(1) / sin(x);
	}
	template<typename T>
	static constexpr T cos(const T& x) noexcept {
		if (std::is_constant_evaluated()) {
			return T(COMPILE_sinTaylorSeries<double>(x + PI<double> / 2.0));
		}
		else {
			return std::cos(x);
		}
	}
	template<typename T>
	static constexpr T sec(const T& x) {
		return T(1) / cos(x);
	}
	template<typename T>
	static constexpr T tan(const T& x) {
		return sin(x) / cos(x);
	}
	template<typename T>
	static constexpr T cot(const T& x) {
		return T(1) / tan(x);
	}

	template<typename T>
	static constexpr T lerp(const T& l, const T& r, float t) {
		return l + (r - l) * t;
	}
	template<typename T>
	static constexpr T max(const T& l, const T& r) {
		if (l > r) {
			return l;
		}
		else {
			return r;
		}
	}
	template<typename T>
	static constexpr T min(const T& l, const T& r) {
		if (l > r) {
			return r;
		}
		else {
			return l;
		}
	}
	template<typename T>
	static constexpr T avg(const T& l, const T& r) {
		return l + (r - l) / T(2);
	}
	template<typename T>
	static constexpr T clamp(const T& val, const T& min, const T& max) {
		if (val < min)
			return min;
		if (val > max)
			return max;
		return val;
	}

	template<typename T>
	static constexpr T random() {
		return static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
	}
	template<typename T>
	static constexpr T random(const T& max) {
		return static_cast<T>(rand()) / static_cast<T>(RAND_MAX / max);
	}
	template<typename T>
	static constexpr T random(const T& min, const T& max) {
		return static_cast<T>(rand()) / static_cast<T>(RAND_MAX / (max - min)) + min;
	}
};
