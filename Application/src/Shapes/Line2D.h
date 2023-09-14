/*
 * Line2D.h
 *
 *  Created on: Jun 30, 2023
 *      Author: nebk2
 */

#ifndef SHAPES_LINE2D_H_
#define SHAPES_LINE2D_H_

#include "Vec2D.h"

class Line2D{
public:
	Line2D();
	Line2D(float x0, float y0, float x1, float y1);
	Line2D(const Vec2D& p0, const Vec2D& p1);

	inline const Vec2D& getP0() const {return mP0;}
	inline const Vec2D& getP1() const {return mP1;}

	inline void setP0(const Vec2D& p0) {mP0 = p0;}
	inline void setP1(const Vec2D& p1) {mP1 = p1;}

	bool operator==(const Line2D& line) const;

	float minDistanceFrom(const Vec2D& p, bool limitToSegment = false) const;
	Vec2D closestPoint(const Vec2D& p, bool limitToSegment = false) const;

	Vec2D midpoint() const;
	float slope() const;
	float length() const;
private:
	Vec2D mP0;
	Vec2D mP1;
};



#endif /* SHAPES_LINE2D_H_ */
