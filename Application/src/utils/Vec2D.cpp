/*
 * Vec2D.cpp
 *
 *  Created on: May 23, 2023
 *      Author: nebk2
 */
#include "Vec2D.h"
#include "Utils.h"

#include <cassert>
#include <cmath>

const Vec2D Vec2D::zero;

std::ostream& operator<<(std::ostream &consoleOut, const Vec2D &vec) {
	std::cout << "X: " << vec.mX << ", Y: " << vec.mY << std::endl;
	return consoleOut;
}

bool Vec2D::operator==(const Vec2D &vec2) const {
	return isEqual(mX, vec2.mX) && isEqual(mY, vec2.mY);
}
bool Vec2D::operator!=(const Vec2D &vec2) const {
	return !(*this == vec2);
}
/**
 * negate vector. unary.
 */
Vec2D Vec2D::operator-() const {
	return Vec2D(-mX, -mY);
}

Vec2D Vec2D::operator*(float scale) const {
	return Vec2D(scale * mX, scale * mY);
}

Vec2D Vec2D::operator/(float scale) const {
	assert(fabsf(scale) > EPSILON);
	return Vec2D(mX / scale, mY / scale);
}

Vec2D& Vec2D::operator*=(float scale) {
	*this = *this * scale;
	return *this;
}

Vec2D& Vec2D::operator/=(float scale) {
	assert(fabsf(scale) > EPSILON);
	*this = *this / scale;
	return *this;
}

Vec2D operator*(float scalar, const Vec2D &vec) {
	return vec * scalar;
}

Vec2D Vec2D::operator-(const Vec2D &vec) const {
	return Vec2D(mX - vec.mX, mY - vec.mY);
}
Vec2D Vec2D::operator+(const Vec2D &vec) const {
	return Vec2D(mX + vec.mX, mY + vec.mY);
}
Vec2D& Vec2D::operator-=(const Vec2D &vec) {
	*this = *this - vec;
	return *this;
}
Vec2D& Vec2D::operator+=(const Vec2D &vec) {
	*this = *this + vec;
	return *this;
}

float Vec2D::mag2() const {
	return mX * mX + mY * mY;
}
float Vec2D::mag() const {
	return sqrt(mag2());
}

Vec2D Vec2D::getUnitVec() const {
	float magg = mag();
	if (magg > EPSILON) {
		return *this / magg;
	}
	return Vec2D::zero;
}

Vec2D& Vec2D::normalize() {
	float magg = mag();
	if (magg > EPSILON) {
		*this /= magg;
	}
	return *this;
}

float Vec2D::distance(const Vec2D &vec) const {
	return (vec - *this).mag();
}

float Vec2D::dot(const Vec2D &vec) const {
	return mX * vec.mX + mY * vec.mY;
}

Vec2D Vec2D::projectOnto(const Vec2D &vec2) const {
	Vec2D unitVec2 = vec2.getUnitVec();
	float dott = dot(unitVec2);
	return unitVec2 * dott;
}

float Vec2D::angleBetween(const Vec2D &vec2) const {
	return acosf(getUnitVec().dot(vec2.getUnitVec()));
}

Vec2D Vec2D::reflect(const Vec2D &normal) const {
	return *this - 2 * projectOnto(normal);
}

void Vec2D::rotate(float angle, const Vec2D &aroundPoint) {
	float cosine = cosf(angle);
	float sine = sinf(angle);

	Vec2D thisVec(mX, mY);

	thisVec -= aroundPoint;

	float xRot = thisVec.mX * cosine - thisVec.mY * sine;
	float yRot = thisVec.mX * sine + thisVec.mY * cosine;

	Vec2D rot = Vec2D(xRot, yRot);

	*this = rot + aroundPoint;
}

Vec2D Vec2D::rotationResult(float angle, const Vec2D &aroundPoint) const {
	float cosine = cosf(angle * (PI/180));
	float sine = sinf(angle * (PI/180));

	Vec2D thisVec(mX, mY);
	thisVec -= aroundPoint;
	float xRot = (thisVec.mX * cosine) - (thisVec.mX * sine);
	float yRot = (thisVec.mY * sine) + (thisVec.mY * cosine);

	Vec2D rot = Vec2D(xRot, yRot);

	return rot + aroundPoint;
}
