/*
 * SoccerBall.cpp
 *
 *  Created on: Nov 8, 2023
 *      Author: nebk2
 */

#include "SoccerBall.h"
#include "Ball.h"
#include "Utils.h"

const float SoccerBall::RADIUS = 3.5f;

SoccerBall::SoccerBall() :
		SoccerBall(Vec2D::zero, SoccerBall::RADIUS) {

}
SoccerBall::SoccerBall(const Vec2D &pos, float radius) :
		mBBox(pos - Vec2D(radius, radius), radius * 2.0f, radius * 2.0f), mVelocity(Vec2D::zero) {
	
}

void SoccerBall::update(uint32_t dt) {
	mBBox.moveBy(mVelocity * millisecondsToSeconds(dt));
}
void SoccerBall::draw(Screen &screen) {

}
void SoccerBall::bounce(const BoundaryEdge &edge) {

}
