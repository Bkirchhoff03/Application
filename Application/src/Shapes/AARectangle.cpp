/*
 * AARectangle.cpp
 *
 *  Created on: Jul 4, 2023
 *      Author: nebk2
 */
#include "AARectangle.h"
#include "Utils.h"
#include <cmath>
AARectangle::AARectangle() :
	AARectangle(Vec2D::zero, Vec2D::zero) {
}
AARectangle::AARectangle(const Vec2D& topLeft, unsigned int width,
	unsigned int height) {
	mPoints.push_back(topLeft);
	mPoints.push_back(Vec2D(topLeft.GetX() + width - 1, topLeft.GetY() + height - 1));
}
AARectangle::AARectangle(const Vec2D& topLeft, const Vec2D& bottomRight) {
	mPoints.push_back(topLeft);
	mPoints.push_back(bottomRight);
}
AARectangle::AARectangle(const AARectangle& rect) {
	mPoints = rect.mPoints;
}

float AARectangle::getWidth() const {
	return getBottomRightPoint().GetX() - getTopLeftPoint().GetX() + 1;
}
float AARectangle::getHeight() const {
	return getBottomRightPoint().GetY() - getTopLeftPoint().GetY() + 1;
}

void AARectangle::moveTo(const Vec2D& position) {
	float width = getWidth();
	float height = getHeight();

	setTopLeftPoint(position);
	setBottomRightPoint(Vec2D(position.GetX() + width - 1, position.GetY() + height - 1));
}

Vec2D AARectangle::getCenterPoint() const {
	return Vec2D(getTopLeftPoint().GetX() + getWidth() / 2.0f, getTopLeftPoint().GetY() + getHeight() / 2.0f);
}

bool AARectangle::intersects(const AARectangle& otherRect) const {
	return !(otherRect.getBottomRightPoint().GetX() < getTopLeftPoint().GetX() ||
		otherRect.getTopLeftPoint().GetX() > getBottomRightPoint().GetX() ||
		otherRect.getBottomRightPoint().GetY() < getTopLeftPoint().GetY() ||
		otherRect.getTopLeftPoint().GetY() > getBottomRightPoint().GetY());
}
bool AARectangle::containsPoint(const Vec2D& point) const {
	bool withinX = point.GetX() >= getTopLeftPoint().GetX() && point.GetX() <= getBottomRightPoint().GetX();
	bool withinY = point.GetY() >= getTopLeftPoint().GetY() && point.GetY() <= getBottomRightPoint().GetY();

	return withinX && withinY;
}

AARectangle AARectangle::inset(const AARectangle& rect, const Vec2D& insets) {
	return AARectangle(rect.getTopLeftPoint() + insets, rect.getWidth() - 2 * insets.GetX(), rect.getHeight() - 2 * insets.GetY());
}

std::vector<Vec2D> AARectangle::getPoints() const {
	std::vector<Vec2D> points;

	points.push_back(mPoints[0]);
	points.push_back(Vec2D(mPoints[1].GetX(), mPoints[0].GetY()));
	points.push_back(mPoints[1]);
	points.push_back(Vec2D(mPoints[0].GetX(), mPoints[1].GetY()));

	return points;
}

bool AARectangle::operator<(const AARectangle& rect) const {
	return mPoints[0].GetX() < rect.getTopLeftPoint().GetX() || mPoints[0].GetY() < rect.getTopLeftPoint().GetY();
}

AARectangle AARectangle::overlappingRectangle(const AARectangle &r1, const AARectangle &r2) {
    float overlapLeft = std::max(r1.getTopLeftPoint().GetX(), r2.getTopLeftPoint().GetX());
    float overlapTop = std::max(r1.getTopLeftPoint().GetY(), r2.getTopLeftPoint().GetY());
    float overlapRight = std::min(r1.getBottomRightPoint().GetX(), r2.getBottomRightPoint().GetX());
    float overlapBottom = std::min(r1.getBottomRightPoint().GetY(), r2.getBottomRightPoint().GetY());
    float overlapWidth = overlapRight - overlapLeft + 1;
    float overlapHeight = overlapBottom - overlapTop + 1;

    if (overlapWidth <= 0 || overlapHeight <= 0) {
        return AARectangle();
    }

    return AARectangle(Vec2D(overlapLeft, overlapTop), overlapWidth, overlapHeight);
}

AARectangle AARectangle::operator-(const AARectangle& rect) const {
	// Check if the rectangles intersect
	
		float overlapLeft = getTopLeftPoint().GetX() > rect.getTopLeftPoint().GetX() ?
			getTopLeftPoint().GetX() : rect.getTopLeftPoint().GetX();
		float overlapTop = getTopLeftPoint().GetY() > rect.getTopLeftPoint().GetY() ?
			getTopLeftPoint().GetY() : rect.getTopLeftPoint().GetY();
		float overlapRight = getTopLeftPoint().GetX() < rect.getTopLeftPoint().GetX() ?
			getTopLeftPoint().GetX() : rect.getTopLeftPoint().GetX();
		float overlapBottom = getTopLeftPoint().GetY() > rect.getTopLeftPoint().GetY() ?
			getTopLeftPoint().GetY() : rect.getTopLeftPoint().GetY();
		float overlapWidth = overlapRight - overlapLeft;
		float overlapHeight = overlapBottom - overlapTop;

		return AARectangle(Vec2D(overlapLeft, overlapTop), overlapWidth, overlapHeight);
	

	// If there is no overlap, return an empty rectangle
	//return AARectangle();

	//return AARectangle(getTopLeftPoint() - rect.getTopLeftPoint(), getWidth() - rect.getWidth(), getHeight() - rect.getHeight());
}
