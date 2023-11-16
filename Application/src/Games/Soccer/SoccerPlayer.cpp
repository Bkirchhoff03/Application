/*
 * SoccerPlayer.cpp
 *
 *  Created on: Sep 25, 2023
 *      Author: nebk2
 */
#include "Utils.h"
#include "SoccerPlayer.h"
#include "SoccerGameUtils.h"
#include <cmath>

void SoccerPlayer::init(const SpriteSheet &spriteSheet, const std::string &animationsPath,
		const Vec2D &intialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Color &spriteColor) {
	mMovementDirection = PLAYER_MOVEMENT_NONE;
	mSprite.init(animationsPath, spriteSheet, spriteColor);
	mUpdateSpriteOnUpdate = updateSpriteOnMovement;
	mMovementSpeed = movementSpeed;
	mDelta = Vec2D::zero;
	mSprite.setPosition(intialPos);
}
void SoccerPlayer::update(uint32_t dt) {
	if (mMovementDirection != PLAYER_MOVEMENT_NONE) {
		Vec2D delta = Vec2D::zero;
		delta = getMovementVector(mMovementDirection) * static_cast<float>(mMovementSpeed);
		mDelta += delta * millisecondsToSeconds(dt);
		if (fabsf(mDelta.GetX()) >= 1) {
			int dx = int(fabsf(mDelta.GetX()));
			if (mDelta.GetX() < 0) {
				mSprite.moveBy(Vec2D(-dx, 0));
				mDelta.SetX(mDelta.GetX() + dx);
			} else {
				mSprite.moveBy(Vec2D(dx, 0));
				mDelta.SetX(mDelta.GetX() - dx);
			}
		} else if (fabsf(mDelta.GetY()) >= 1) {
			int dy = int(fabsf(mDelta.GetY()));

			if (mDelta.GetY() < 0) {
				mSprite.moveBy(Vec2D(0, -dy));
				mDelta.SetY(mDelta.GetY() + dy);
			} else {
				mSprite.moveBy(Vec2D(0, dy));
				mDelta.SetY(mDelta.GetY() - dy);
			}
		}

		mSprite.update(dt);
	}

	if (mUpdateSpriteOnUpdate && mMovementDirection == PLAYER_MOVEMENT_NONE) {
		mSprite.update(dt);
	}
}

void SoccerPlayer::draw(Screen &screen) {
	mSprite.draw(screen);
}

void SoccerPlayer::stop() {
	setMovementDirection(PLAYER_MOVEMENT_NONE);
	mSprite.stop();
}
AARectangle SoccerPlayer::getDribbleBoundingBox() const {
	return AARectangle::inset(getBoundingBox(), Vec2D(3, 3));
}

void SoccerPlayer::setAnimation(const std::string &animationName, bool looped) {
	mSprite.setAnimation(animationName, looped);
}


