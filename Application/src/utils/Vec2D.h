/*
 * Vec2D.h
 *
 *  Created on: May 23, 2023
 *      Author: nebk2
 */

#ifndef UTILS_VEC2D_H_
#define UTILS_VEC2D_H_

#include <iostream>
class Vec2D{
public:

	static const Vec2D zero;

	Vec2D(): Vec2D(0,0){}
	Vec2D(float x, float y): mX(x), mY(y){}

	void SetX(float x){mX = x;}
	void SetY(float y){mY = y;}
	inline float GetX() const {return mX;}
	inline float GetY() const {return mY;}

	friend std::ostream& operator<<(std::ostream& consoleOut, const Vec2D& vec);

	bool operator==(const Vec2D& vec2) const;
	bool operator!=(const Vec2D& vec2) const;

	Vec2D operator-() const;
	Vec2D operator*(float scale) const;
	Vec2D operator/(float scale) const;

	Vec2D& operator*=(float scale);
	Vec2D& operator/=(float scale);

	Vec2D operator-(const Vec2D& vec) const;
	Vec2D operator+(const Vec2D& vec) const;
	Vec2D& operator-=(const Vec2D& vec);
	Vec2D& operator+=(const Vec2D& vec);

	float mag2() const;
	float mag() const;

	Vec2D getUnitVec() const;
	Vec2D& normalize();

	float distance(const Vec2D& vec) const;
	float dot(const Vec2D& vec) const;

	Vec2D projectOnto(const Vec2D& vec2) const;

	float angleBetween(const Vec2D& vec2) const;

	Vec2D reflect(const Vec2D& normal) const;

	void rotate(float angle, const Vec2D& aroundPoint);
	Vec2D rotationResult(float angle, const Vec2D& aroundPoint) const;

	friend Vec2D operator*(float scalar, const Vec2D& vec);
private:
	float mX, mY;
};






#endif /* UTILS_VEC2D_H_ */
