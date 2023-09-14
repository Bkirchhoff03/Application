/*
 * Triangle.cpp
 *
 *  Created on: Jul 3, 2023
 *      Author: nebk2
 */

#include "Triangle.h"
#include <cmath>
#include "Utils.h"

Triangle::Triangle() :
		Triangle(Vec2D::zero, Vec2D::zero, Vec2D::zero) {

}
Triangle::Triangle(const Vec2D &p0, const Vec2D &p1, const Vec2D &p2) {
	mPoints.push_back(p0);
	mPoints.push_back(p1);
	mPoints.push_back(p2);
}

Vec2D Triangle::getCenterPoint() const {
	return Vec2D((mPoints[0].GetX() + mPoints[1].GetX() + mPoints[2].GetX()) /3.0f, (mPoints[0].GetY() + mPoints[1].GetY() + mPoints[2].GetY())/3.0f);
}

float Triangle::area() const{
	return area(getP0(), getP1(), getP2());
}

bool Triangle::containsPoint(const Vec2D &p) const{
	float thisArea = area();

	float a1 = area(p, getP1(), getP2());

	float a2 = area(getP0(), p, getP2());

	float a3 = area(getP0(), getP1(), p);
	return isEqual(thisArea, a1 + a2 + a3);
}
float Triangle::area(const Vec2D &p0, const Vec2D &p1, const Vec2D &p2) const{
	return fabsf((p0.GetX() * (p1.GetY() - p2.GetY()) + p1.GetX()*(p2.GetY()-p0.GetY()) + p2.GetX()*(p0.GetY()-p1.GetY()))/2.0f);
}

void Triangle::moveTo(const Vec2D& position){
	Vec2D dif = position - getCenterPoint();
	moveBy(dif);
}

