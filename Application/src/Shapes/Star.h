/*
 * Star.h
 *
 *  Created on: Jul 6, 2023
 *      Author: nebk2
 */

#ifndef SHAPES_STAR_H_
#define SHAPES_STAR_H_
#include "Shape.h"
#include "Triangle.h"
class Star: public Shape {
public:
	Star();
	Star(const Vec2D &p0, float radius);

	virtual std::vector<Vec2D> getPoints() const override;


	virtual Vec2D getCenterPoint() const override;
	virtual void moveTo(const Vec2D &position) override;
	void rotate(float degree);
	bool containsPoint(const Vec2D &p) const;
private:
	float getMiddleRadius() const;
	float radius;
};

#endif /* SHAPES_STAR_H_ */
