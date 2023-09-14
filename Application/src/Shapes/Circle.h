/*
 * Circle.h
 *
 *  Created on: Jul 4, 2023
 *      Author: nebk2
 */

#ifndef SHAPES_CIRCLE_H_
#define SHAPES_CIRCLE_H_
#include "Shape.h"
class Circle: public Shape{
public:
	Circle();
	Circle(const Vec2D& center, float radius);

	inline virtual Vec2D getCenterPoint() const override {return mPoints[0];}
	inline float getRadius() const {return mRadius;}
	inline void setRadius(float radius) {mRadius = radius;}
	virtual void moveTo(const Vec2D& position) override {mPoints[0] = position;}

	bool intersects(const Circle& otherCircle) const;
	bool containsPoint(const Vec2D& point) const;

private:
	float mRadius;
};



#endif /* SHAPES_CIRCLE_H_ */
