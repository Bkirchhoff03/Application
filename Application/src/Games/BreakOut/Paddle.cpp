/*
 * Paddle.cpp
 *
 *  Created on: Jul 15, 2023
 *      Author: nebk2
 */

#include "Paddle.h"
#include "Screen.h"
#include "Utils.h"
#include "BoundaryEdge.h"
#include "Ball.h"
#include <cassert>

void Paddle::init(const AARectangle &rect, const AARectangle &boundary) {
	Excluder::init(rect);
	mBoundary = boundary;
	mDirection = 0;
}

void Paddle::update(uint32_t dt, Ball& ball) {
	if(getAARectangle().containsPoint(ball.getPosition())){
		Vec2D pointOnEdge;
		ball.makeFlushWithEdge(getEdge(BOTTOM_EDGE), pointOnEdge, true);
	}
	if (mDirection != 0) {
		Vec2D dir;
		if ((mDirection & PaddleDirection::LEFT) == PaddleDirection::LEFT
				&& (mDirection & PaddleDirection::RIGHT)
						== PaddleDirection::RIGHT) {
			dir = Vec2D::zero;
		} else if (mDirection == PaddleDirection::LEFT) {
			dir = LEFT_DIR;
		} else {
			dir = RIGHT_DIR;
		}
		Vec2D dx = dir * VELOCITY * millisecondsToSeconds(dt);
		moveBy(dx);

		const AARectangle &aaRect = getAARectangle();

		if (isGreaterThanOrEqual(mBoundary.getTopLeftPoint().GetX(),
				aaRect.getTopLeftPoint().GetX())) {
			moveTo(
					Vec2D(mBoundary.getTopLeftPoint().GetX(),
							aaRect.getTopLeftPoint().GetY()));
		} else if (isGreaterThanOrEqual(aaRect.getBottomRightPoint().GetX(),
				mBoundary.getBottomRightPoint().GetX())) {
			moveTo(
					Vec2D(
							mBoundary.getBottomRightPoint().GetX()
									- aaRect.getWidth(),
							aaRect.getTopLeftPoint().GetY()));
		}
	}
}

void Paddle::draw(Screen &screen) {
	screen.draw(getAARectangle(), Color::blue(), true, Color::blue());
}

bool Paddle::bounce(Ball &ball) {
	BoundaryEdge edge;
	if (hasCollided(ball.getBoundingRect(), edge)) {
		Vec2D pointOnEdge;

		ball.makeFlushWithEdge(edge, pointOnEdge, true);

		if (edge.edge == getEdge(TOP_EDGE).edge) {
			float edgeLength = edge.edge.length();
			assert(!isEqual(edgeLength, 0));
			float tx = (pointOnEdge.GetX() - edge.edge.getP0().GetX())
					/ edgeLength;

			if (((tx <= CORNER_BOUNCE_AMT) && (ball.getVelocity().GetX() > 0))
					|| (tx >= (1.0f - CORNER_BOUNCE_AMT)
							&& ball.getVelocity().GetX() < 0)) {
				ball.setVelocity(-ball.getVelocity());
				return true;
			}
		}
		ball.setVelocity(ball.getVelocity().reflect(edge.normal));
		return true;
	}
	return false;
}

