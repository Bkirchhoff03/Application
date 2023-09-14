/*
 * Utils.cpp
 *
 *  Created on: May 23, 2023
 *      Author: nebk2
 */
#include "Utils.h"
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include <cctype>

bool isEqual(float x, float y) {
	return fabsf(x - y) < EPSILON;
}

bool isGreaterThanOrEqual(float x, float y) {
	return x > y || isEqual(x, y);
}

bool isLessThanOrEqual(float x, float y) {
	return x < y || isEqual(x, y);
}

float millisecondsToSeconds(unsigned int milliseconds) {
	return static_cast<float>(milliseconds) / 1000.0f;
}

uint32_t getIndex(unsigned int width, unsigned int r, unsigned int c) {
	return r * width + c;
}

bool stringCompare(const std::string &a, const std::string &b) {
	if (a.length() == b.length()) {
		return std::equal(b.begin(), b.end(), a.begin(), [](unsigned char a, unsigned char b) {
			return std::tolower(a) == std::tolower(b);
		});
	}
	return false;
}

float clamp(float val, float min, float max) {
	if (val > max) {
		return max;
	} else if (val < min) {
		return min;
	}
	return val;
}

