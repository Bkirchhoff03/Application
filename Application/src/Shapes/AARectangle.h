/*
 * AARectangle.h
 *
 *  Created on: Jul 4, 2023
 *      Author: nebk2
 */

#ifndef SHAPES_AARECTANGLE_H_
#define SHAPES_AARECTANGLE_H_

#include "Shape.h"

class AARectangle: public Shape{
public:
	AARectangle();
	AARectangle(const Vec2D& topLeft, unsigned int width, unsigned int height);
	AARectangle(const Vec2D& topLeft, const Vec2D& bottomRight);
	AARectangle(const AARectangle& rect);

	inline void setTopLeftPoint(const Vec2D& topLeft) {mPoints[0] = topLeft;}
	inline void setBottomRightPoint(const Vec2D& bottomRight) {mPoints[1] = bottomRight;}
	static AARectangle overlappingRectangle(const AARectangle& r1, const AARectangle& r2);
	Vec2D getTopLeftPoint() const {return mPoints[0];}
	Vec2D getBottomRightPoint() const {return mPoints[1];}
	bool operator<(const AARectangle& rect) const;
	AARectangle operator-(const AARectangle& rect) const;
	float getWidth() const;
	float getHeight() const;

	virtual void moveTo(const Vec2D& position) override;

	virtual Vec2D getCenterPoint() const override;

	bool intersects(const AARectangle& otherRect) const;
	bool containsPoint(const Vec2D& point) const;

	static AARectangle inset(const AARectangle &rect, const Vec2D &insets);

	virtual std::vector<Vec2D> getPoints() const override;

};

#endif /* SHAPES_AARECTANGLE_H_ */
