/*
 * Circle.cpp
 *
 *  Created on: Jul 4, 2023
 *      Author: nebk2
 */
#include "Circle.h"
#include "Utils.h"
Circle::Circle() :
		Circle(Vec2D::zero, 0) {
}
Circle::Circle(const Vec2D &center, float radius) :
		mRadius(radius) {
	mPoints.push_back(center);
}

bool Circle::intersects(const Circle &otherCircle) const {
	return getCenterPoint().distance(otherCircle.getCenterPoint()) < (mRadius + otherCircle.mRadius);
}
bool Circle::containsPoint(const Vec2D &point) const{
	return isLessThanOrEqual(getCenterPoint().distance(point), mRadius);
}

