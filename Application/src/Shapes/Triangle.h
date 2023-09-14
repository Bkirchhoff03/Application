/*
 * Triangle.h
 *
 *  Created on: Jul 3, 2023
 *      Author: nebk2
 */

#ifndef SHAPES_TRIANGLE_H_
#define SHAPES_TRIANGLE_H_

#include "Shape.h"

class Triangle: public Shape{
public:
	Triangle();
	Triangle(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2);

	inline void setP0(const Vec2D& p0) {mPoints[0] = p0;}
	inline void setP1(const Vec2D& p1) {mPoints[1] = p1;}
	inline void setP2(const Vec2D& p2) {mPoints[2] = p2;}

	inline Vec2D getP0() const {return mPoints[0];}
	inline Vec2D getP1() const {return mPoints[1];}
	inline Vec2D getP2() const {return mPoints[2];}

	virtual Vec2D getCenterPoint() const override;
	virtual void moveTo(const Vec2D& position) override;
	float area() const;

	bool containsPoint(const Vec2D& p) const;
private:
	float area(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2) const;
};



#endif /* SHAPES_TRIANGLE_H_ */
