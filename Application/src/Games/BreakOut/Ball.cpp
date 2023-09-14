/*
 * Ball.cpp
 *
 *  Created on: Jul 16, 2023
 *      Author: nebk2
 */

#include "Ball.h"
#include "Utils.h"
#include "Screen.h"
#include "Circle.h"
#include "BoundaryEdge.h"

const float Ball::RADIUS = 5.0f;

Ball::Ball():Ball(Vec2D::zero, Ball::RADIUS) {

}
Ball::Ball(const Vec2D &pos, float radius):mBBox(pos - Vec2D(radius, radius), radius*2.0f, radius*2.0f), mVelocity(Vec2D::zero) {

}

void Ball::update(uint32_t dt) {
	mBBox.moveBy(mVelocity * millisecondsToSeconds(dt));
}
void Ball::draw(Screen &screen) {
	Circle circ = {mBBox.getCenterPoint(), getRadius()};
	screen.draw(circ, Color::red(), true, Color::red());
}
void Ball::makeFlushWithEdge(const BoundaryEdge &edge, Vec2D &pointOnEdge,
		bool limitToEdge) {
	if(edge.normal == DOWN_DIR){
		mBBox.moveTo(Vec2D(mBBox.getTopLeftPoint().GetX(), edge.edge.getP0().GetY() + edge.normal.GetY()));
	}else if(edge.normal == UP_DIR){
		mBBox.moveTo(Vec2D(mBBox.getTopLeftPoint().GetX(), edge.edge.getP0().GetY() - mBBox.getHeight()));
	}else if(edge.normal == RIGHT_DIR){
		mBBox.moveTo(Vec2D(edge.edge.getP0().GetX() + edge.normal.GetX(), mBBox.getTopLeftPoint().GetY()));
	}else if(edge.normal == LEFT_DIR){
		mBBox.moveTo(Vec2D(edge.edge.getP0().GetX() - mBBox.getWidth(), mBBox.getTopLeftPoint().GetY()));
	}

	pointOnEdge= edge.edge.closestPoint(mBBox.getCenterPoint(), limitToEdge);
}

void Ball::bounce(const BoundaryEdge& edge){
	Vec2D pointOnEdge;

	makeFlushWithEdge(edge, pointOnEdge, false);

	mVelocity = mVelocity.reflect(edge.normal);
}


void Ball::moveTo(const Vec2D &point) {
	mBBox.moveTo(point - Vec2D(getRadius(), getRadius()));
}

