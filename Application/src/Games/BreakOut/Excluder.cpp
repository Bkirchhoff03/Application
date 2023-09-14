/*
 * Excluder.cpp
 *
 *  Created on: Jul 14, 2023
 *      Author: nebk2
 */

#include "Excluder.h"
#include "Utils.h"
#include <cmath>
#include <cassert>

void Excluder::init(const AARectangle &rect, bool reverseNormals) {
	mAARect = rect;
	mReverseNormals = reverseNormals;
	setupEdges();
}
bool Excluder::hasCollided(const AARectangle &rect, BoundaryEdge &edge) const {
	if (mAARect.intersects(rect)) {
		float yMin =
				mAARect.getTopLeftPoint().GetY()
						>= (rect.getTopLeftPoint().GetY()) ?
						mAARect.getTopLeftPoint().GetY() :
						rect.getTopLeftPoint().GetY();
		float yMax =
				mAARect.getBottomRightPoint().GetY()
						>= (rect.getBottomRightPoint().GetY()) ?
						mAARect.getBottomRightPoint().GetY() :
						rect.getBottomRightPoint().GetY();

		float ySize = yMax - yMin;

		float xMin =
				mAARect.getTopLeftPoint().GetX()
						>= (rect.getTopLeftPoint().GetX()) ?
						mAARect.getTopLeftPoint().GetX() :
						rect.getTopLeftPoint().GetX();
		float xMax =
				mAARect.getBottomRightPoint().GetX()
						>= (rect.getBottomRightPoint().GetX()) ?
						mAARect.getBottomRightPoint().GetX() :
						rect.getBottomRightPoint().GetX();

		float xSize = xMax - xMin;

		if (xSize > ySize) {
			if (rect.getCenterPoint().GetY()
					> mAARect.getCenterPoint().GetY()) {
				edge = mEdges[BOTTOM_EDGE];
			} else {
				edge = mEdges[TOP_EDGE];
			}
		} else {
			if (rect.getCenterPoint().GetX()
					< mAARect.getCenterPoint().GetX()) {
				edge = mEdges[LEFT_EDGE];
			} else {
				edge = mEdges[RIGHT_EDGE];
			}
		}
		return true;
	}
	return false;
}
Vec2D Excluder::getCollisionOffset(const AARectangle &rect) const {
	BoundaryEdge edge;
	Vec2D offset = Vec2D::zero;

	if (hasCollided(rect, edge)) {
		float yMin =
				mAARect.getTopLeftPoint().GetY()
						>= (rect.getTopLeftPoint().GetY()) ?
						mAARect.getTopLeftPoint().GetY() :
						rect.getTopLeftPoint().GetY();
		float yMax =
				mAARect.getBottomRightPoint().GetY()
						>= (rect.getBottomRightPoint().GetY()) ?
						mAARect.getBottomRightPoint().GetY() :
						rect.getBottomRightPoint().GetY();

		float ySize = yMax - yMin;

		float xMin =
				mAARect.getTopLeftPoint().GetX()
						>= (rect.getTopLeftPoint().GetX()) ?
						mAARect.getTopLeftPoint().GetX() :
						rect.getTopLeftPoint().GetX();
		float xMax =
				mAARect.getBottomRightPoint().GetX()
						>= (rect.getBottomRightPoint().GetX()) ?
						mAARect.getBottomRightPoint().GetX() :
						rect.getBottomRightPoint().GetX();

		float xSize = xMax - xMin;

		if(!isEqual(edge.normal.GetY(), 0)){
			offset = (ySize + 1) * edge.normal;
		}else{
			offset = (xSize + 1) * edge.normal;
		}
	}
	return Vec2D::zero;
}
void Excluder::moveBy(const Vec2D &delta) {
	mAARect.moveBy(delta);
	setupEdges();
}
void Excluder::moveTo(const Vec2D &point) {
	mAARect.moveTo(point);
	setupEdges();
}
const BoundaryEdge& Excluder::getEdge(EdgeType edge) const {
	assert(edge != NUM_EDGES);
	return mEdges[edge];
}
void Excluder::setupEdges() {
	mEdges[TOP_EDGE].edge = { mAARect.getTopLeftPoint().GetX(),
			mAARect.getTopLeftPoint().GetY(),
			mAARect.getBottomRightPoint().GetX(),
			mAARect.getTopLeftPoint().GetY() };
	mEdges[TOP_EDGE].normal = UP_DIR;

	mEdges[LEFT_EDGE].edge = { mAARect.getTopLeftPoint().GetX(),
			mAARect.getTopLeftPoint().GetY(), mAARect.getTopLeftPoint().GetX(),
			mAARect.getBottomRightPoint().GetY() };
	mEdges[LEFT_EDGE].normal = LEFT_DIR;

	mEdges[BOTTOM_EDGE].edge = { mAARect.getTopLeftPoint().GetX(),
			mAARect.getBottomRightPoint().GetY(),
			mAARect.getBottomRightPoint().GetX(),
			mAARect.getBottomRightPoint().GetY() };
	mEdges[BOTTOM_EDGE].normal = DOWN_DIR;

	mEdges[RIGHT_EDGE].edge = { mAARect.getBottomRightPoint().GetX(),
			mAARect.getTopLeftPoint().GetY(),
			mAARect.getBottomRightPoint().GetX(),
			mAARect.getBottomRightPoint().GetY() };
	mEdges[RIGHT_EDGE].normal = RIGHT_DIR;

	if (mReverseNormals) {
		for (auto &edge : mEdges) {
			edge.normal = -edge.normal;
		}
	}
}

