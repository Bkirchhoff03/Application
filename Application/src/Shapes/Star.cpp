/*
 * Star.cpp
 *
 *  Created on: Jul 6, 2023
 *      Author: nebk2
 */
#include "Star.h"
#include "Vec2D.h"
#include "Utils.h"
#include "Circle.h"
#include <cmath>

Star::Star() {
	Star(Vec2D::zero, 0.0f);
	radius = 0;
}
Star::Star(const Vec2D &center, float radius) {
	this->radius = radius;
	Vec2D p1, p2, p3, p4, p5;
	const float x = center.GetX();
	const float y = center.GetY();
	p1.SetX(x);
	p1.SetY(y-radius);

	p2.SetX(x-(radius * cosf(18 * (PI / 180))));
	p2.SetY(y-(radius * sinf(18 * (PI / 180))));

	p3.SetX(x-(radius * cosf(306 * (PI / 180))));
	p3.SetY(y-(radius * sinf(306 * (PI / 180))));

	p4.SetX(x-(radius * cosf(234 * (PI / 180))));
	p4.SetY(y-(radius * sinf(234 * (PI / 180))));

	p5.SetX(x-(radius * cosf(162 * (PI / 180))));
	p5.SetY(y-(radius * sinf(162 * (PI / 180))));

	Vec2D ip1, ip2, ip3, ip4, ip5;
	float mRad = getMiddleRadius();

	ip1.SetX(x-(mRad * cosf(54 * (PI / 180))));
	ip1.SetY(y-(mRad * sinf(54 * (PI / 180))));

	ip2.SetX(x-(mRad * cosf(342 * (PI / 180))));
	ip2.SetY(y-(mRad * sinf(342 * (PI / 180))));

	ip3.SetX(x);
	ip3.SetY(y+mRad);

	ip4.SetX(x-(mRad * cosf(198 * (PI / 180))));
	ip4.SetY(y-(mRad * sinf(198 * (PI / 180))));

	ip5.SetX(x-(mRad * cosf(126 * (PI / 180))));
	ip5.SetY(y-(mRad * sinf(126 * (PI / 180))));

	mPoints.push_back(center);
	mPoints.push_back(p1);
	mPoints.push_back(p2);
	mPoints.push_back(p3);
	mPoints.push_back(p4);
	mPoints.push_back(p5);

	mPoints.push_back(ip1);
	mPoints.push_back(ip2);
	mPoints.push_back(ip3);
	mPoints.push_back(ip4);
	mPoints.push_back(ip5);
}
Vec2D Star::getCenterPoint() const {
	return mPoints[0];
}
void Star::moveTo(const Vec2D &position) {
	mPoints[0] -= getCenterPoint();
	mPoints[1] -= getCenterPoint();
	mPoints[2] -= getCenterPoint();
	mPoints[3] -= getCenterPoint();
	mPoints[4] -= getCenterPoint();
	mPoints[5] -= getCenterPoint();

	mPoints[0] += position;
	mPoints[1] += position;
	mPoints[2] += position;
	mPoints[3] += position;
	mPoints[4] += position;
	mPoints[5] += position;

	mPoints[6] += position;
	mPoints[7] += position;
	mPoints[8] += position;
	mPoints[9] += position;
	mPoints[10] += position;
}

bool Star::containsPoint(const Vec2D &p) const {
	float mRad = getMiddleRadius();
	Circle c { mPoints[0], this->radius };
	if (!c.containsPoint(p)) {
		return false;
	}
	Circle c2 { mPoints[0], mRad };
	if (c2.containsPoint(p)) {
		return true;
	}

	for (int i = 0; i < 5; i++) {
		Triangle t;
		if (i == 0) {
			t.setP0(mPoints[i + 1]);
			t.setP1(Vec2D(mPoints[10].GetX(), mPoints[10].GetY()));
			t.setP2(Vec2D(mPoints[i + 6].GetX(), mPoints[i + 6].GetY()));
		} else {
			t.setP0(mPoints[i + 1]);
			t.setP1(Vec2D(mPoints[i + 5].GetX(), mPoints[i + 5].GetY()));
			t.setP2(Vec2D(mPoints[i + 6].GetX(), mPoints[i + 6].GetY()));
		}
		if (t.containsPoint(p)) {
			return true;
		}
	}
	return false;
}

float Star::getMiddleRadius() const {
	return radius * .382;
}

void Star::rotate(float degree) {
	mPoints[1].rotate(degree, mPoints[0]);
	mPoints[2].rotate(degree, mPoints[0]);
	mPoints[3].rotate(degree, mPoints[0]);
	mPoints[4].rotate(degree, mPoints[0]);
	mPoints[5].rotate(degree, mPoints[0]);
	mPoints[6].rotate(degree, mPoints[0]);
	mPoints[7].rotate(degree, mPoints[0]);
	mPoints[8].rotate(degree, mPoints[0]);
	mPoints[9].rotate(degree, mPoints[0]);
	mPoints[10].rotate(degree, mPoints[0]);
}

std::vector<Vec2D> Star::getPoints() const{
	std::vector<Vec2D> points;
	points.push_back(mPoints[1]);
	points.push_back(mPoints[2]);
	points.push_back(mPoints[3]);
	points.push_back(mPoints[4]);
	points.push_back(mPoints[5]);
	points.push_back(mPoints[6]);
	points.push_back(mPoints[7]);
	points.push_back(mPoints[8]);
	points.push_back(mPoints[9]);
	points.push_back(mPoints[10]);
	return points;
}
