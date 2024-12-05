#pragma once
#include <cmath>
#include "float4.h"

//Primes generated with https://asecuritysite.com/encryption/nprimes?y=64
constexpr static uint64_t mul = 16854404399406269837;
constexpr static uint64_t mulX = 18347832797054982829;
constexpr static uint64_t mulY = 13608807687488009933;
constexpr static uint64_t mulZ = 18196416937613063183;

constexpr static uint64_t add = 15830533094606812669;
constexpr static uint64_t addX = 16819764892729833949;
constexpr static uint64_t addY = 14836742468406874129;
constexpr static uint64_t addZ = 14430369713846128183;

struct noise
{
	static uint64_t value(uint64_t seed) {
		state value = uint64_t(seed);
		value.mutate();
		return value.s_uint64;
	}
	static int64_t value(int64_t seed) {
		state value = uint64_t(seed);
		value.mutate();
		return value.s_int64;
	}
	static uint32_t value(uint32_t seed) {
		state value = uint64_t(seed);
		value.mutate();
		return value.s_uint32;
	}
	static int32_t value(int32_t seed) {
		state value = uint64_t(seed);
		value.mutate();
		return value.s_int32;
	}
	static uint16_t value(uint16_t seed) {
		state value = uint64_t(seed);
		value.mutate();
		return value.s_uint16;
	}
	static int16_t value(int16_t seed) {
		state value = uint64_t(seed);
		value.mutate();
		return value.s_int16;
	}
	static uint8_t value(uint8_t seed) {
		state value = uint64_t(seed);
		value.mutate();
		return value.s_uint8;
	}
	static int8_t value(int8_t seed) {
		state value = uint64_t(seed);
		value.mutate();
		return value.s_int8;
	}
	static double value(double seed) {
		state value = seed;
		value.mutate();
		double integer;
		return std::modf(value.s_double, &integer);
	}
	static float value(float seed) {
		state value = seed;
		value.mutate();
		float integer;
		return std::modf(value.s_double, &integer);
	}

private:
	union state {
		uint64_t s_uint64;
		int64_t s_int64;
		uint32_t s_uint32;
		int32_t s_int32;
		uint16_t s_uint16;
		int16_t s_int16;
		uint8_t s_uint8;
		int8_t s_int8;
		double s_double;
		float s_float;

		state(uint64_t state) {
			s_uint64 = state ^ mul;
		}
		state(double state) {
			s_double = state;
			s_uint64 ^= mul;
		}
		state(float state) {
			s_float = state;
			s_uint64 ^= mul;
		}

		void mutate() {
			//s_uint64 = s_uint64 * mul + add;
			s_uint64 ^= s_uint64 << 13;
			s_uint64 ^= s_uint64 >> 17;
			s_uint64 ^= s_uint64 << 5;
		}
	};
};
