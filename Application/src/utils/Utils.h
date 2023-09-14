/*
 * Utils.h
 *
 *  Created on: May 23, 2023
 *      Author: nebk2
 */

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_
#include <string>
typedef unsigned uint32_t;
static const float EPSILON = 0.0001f;

const float PI = 3.14159274101257324219f;
const float TWO_PI = 2.0f * PI;

bool isEqual(float x, float y);

bool isGreaterThanOrEqual(float x, float y);

bool isLessThanOrEqual(float x, float y);

float millisecondsToSeconds(uint32_t milliseconds);

uint32_t getIndex(unsigned int width, unsigned int r, unsigned int c);

//STrIng == stRing
bool stringCompare(const std::string &a, const std::string &b);

float clamp(float val, float min, float max);

struct Size {
	unsigned int width = 0, height = 0;
};

#endif /* UTILS_UTILS_H_ */
