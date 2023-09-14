/*
 * Shape.h
 *
 *  Created on: Jul 3, 2023
 *      Author: nebk2
 */

#ifndef SHAPES_SHAPE_H_
#define SHAPES_SHAPE_H_

#include "Vec2D.h"
#include <vector>

class Shape{
public:
	virtual Vec2D getCenterPoint() const = 0;
	virtual ~Shape() {}
	inline virtual std::vector<Vec2D> getPoints() const {return mPoints;}
	void moveBy(const Vec2D& deltaOffset);
	virtual void moveTo(const Vec2D& position) = 0;
protected:
	std::vector<Vec2D> mPoints;
};

#endif /* SHAPES_SHAPE_H_ */
