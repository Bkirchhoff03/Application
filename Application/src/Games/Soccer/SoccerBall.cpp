/*
 * SoccerBall.cpp
 *
 *  Created on: Nov 8, 2023
 *      Author: nebk2
 */

#include "SoccerBall.h"
#include "Ball.h"
#include "Utils.h"
#include "SoccerGameUtils.h"
#include "Circle.h"
#include "Screen.h"
#include "SoccerPlayer.h"
#include "Vec2D.h"
#include "BoundaryEdge.h"
#include "TeamAgainst.h"
#include <math.h>

const float SoccerBall::RADIUS = 3.5f;

SoccerBall::SoccerBall() :
	SoccerBall(Vec2D::zero, SoccerBall::RADIUS) {
}
SoccerBall::SoccerBall(SoccerBall& soccerBall) {
	mBBox = soccerBall.mBBox;
	mVelocity = soccerBall.mVelocity;
	mState = soccerBall.mState;
	mSpawnPosition = soccerBall.mSpawnPosition;
}
SoccerBall::SoccerBall(const Vec2D& pos, float radius) :
	mBBox(pos - Vec2D(radius, radius), radius * 2.0f, radius * 2.0f), mVelocity(Vec2D::zero) {
	mState = SOCCER_BALL_STATE_IN_PLAY_AT_REST;
	mSpawnPosition = pos;
}

void SoccerBall::update(uint32_t dt) {
	if (mState == SOCCER_BALL_STATE_IN_PLAY_MOVING) {
		mVelocity -= mVelocity.getUnitVec();
		//mVelocity *= (1.0f - (SoccerBall::FRICTION * millisecondsToSeconds(dt)));
	}
	else {
		mVelocity = Vec2D::zero;
		mState = SOCCER_BALL_STATE_IN_PLAY_AT_REST;
	}
	mBBox.moveBy(mVelocity * millisecondsToSeconds(dt));

}
void SoccerBall::draw(Screen& screen) {
	screen.draw(Circle(mBBox.getCenterPoint(), (mBBox.getWidth() / 2.0f)), Color::black(), true, Color::black());
}
void SoccerBall::bounce(const BoundaryEdge& edge) {
	Vec2D pointOnEdge;

	makeFlushWithEdge(edge, pointOnEdge, false);

	mVelocity = mVelocity.reflect(edge.normal);
}
void SoccerBall::resetToFirstPosition() {
	mBBox.moveTo(mSpawnPosition);
}

void SoccerBall::bounceOffOfSoccerPlayer(SoccerPlayer soccerPlayer) {
	if (mState == SOCCER_BALL_STATE_IN_PLAY_AT_REST) {
		mState = SOCCER_BALL_STATE_IN_PLAY_MOVING;
	}
	float playerSpeed = static_cast<float>(soccerPlayer.getMovementSpeed());
	Vec2D playerVelocity = getMovementVector(soccerPlayer.getMovementDirection());

	// Calculate the new velocity of the ball after bouncing off the player
	Vec2D newVelocity = playerVelocity * playerSpeed * 2;

	// Update the ball's velocity
	mVelocity = newVelocity;
}
