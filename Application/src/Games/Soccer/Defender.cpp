/*
 * Defender.cpp
 *
 *  Created on: Sep 25, 2023
 *      Author: nebk2
 */

#include "Defender.h"

Defender::Defender() :
		mDelegate(nullptr), mCanChangeDirection(true), mState(PLAYER_STOPPED) {

}
void Defender::init(const SpriteSheet &spriteSheet, const std::string &animationsPath, const Vec2D &intialPos,
		uint32_t movementSpeed, bool updateSpriteOnMovement, const Color &spriteColor) {
	SoccerPlayer::init(spriteSheet, animationsPath, intialPos, movementSpeed, updateSpriteOnMovement, spriteColor);
	mInitialPos = intialPos;
	resetToFirstPosition();
}
void Defender::update(uint32_t dt) {
	Vec2D pos = position();
	SoccerPlayer::update(dt);
	mCanChangeDirection = pos != position();
}
void Defender::resetToFirstPosition() {
	mSprite.setPosition(mInitialPos);
	setMovementDirection(PLAYER_MOVEMENT_NONE);
	setDefenderState(PLAYER_STOPPED);
	mCanChangeDirection = true;
	if (mDelegate) {
		mDelegate->defenderWasResetToZone();
	}

}
void Defender::setMovementDirection(PlayerMovement direction) {
	SoccerPlayer::setMovementDirection(direction);
	PlayerMovement movementDir = getMovementDirection();

	if (mState == PLAYER_STOPPED || mState == PLAYER_SPRINTING || mState == PLAYER_RUNNING || mState == PLAYER_JOGGING
			|| mState == PLAYER_SLIDING || mState == PLAYER_TACKLING) {
		if (movementDir == PLAYER_MOVEMENT_RIGHT) {
			setAnimation("defender_movement_right", true);
		} else if (movementDir == PLAYER_MOVEMENT_UP) {
			setAnimation("defender_movement_up", true);
		} else if (movementDir == PLAYER_MOVEMENT_DOWN) {
			setAnimation("defender_movement_down", true);
		} else if (movementDir == PLAYER_MOVEMENT_LEFT) {
			setAnimation("defender_movement_left", true);
		}
	} else if (mState == PLAYER_STOPPED_WITH_BALL || mState == PLAYER_SPRINTING_WITH_BALL
			|| mState == PLAYER_RUNNING_WITH_BALL || mState == PLAYER_JOGGING_WITH_BALL) {
		if (movementDir == PLAYER_MOVEMENT_RIGHT) {
			setAnimation("defender_movement_with_ball_right", true);
		} else if (movementDir == PLAYER_MOVEMENT_UP) {
			setAnimation("defender_movement_with_ball_up", true);
		} else if (movementDir == PLAYER_MOVEMENT_DOWN) {
			setAnimation("defender_movement_with_ball_down", true);
		} else if (movementDir == PLAYER_MOVEMENT_LEFT) {
			setAnimation("defender_movement_with_ball_left", true);
		}
	}

}
void Defender::stop() {
	setMovementDirection(PLAYER_MOVEMENT_NONE);
}
void Defender::setDefenderState(PlayerState state) {
	if (mDelegate) {
		mDelegate->defenderDelegateDefenderStateChangeTo(mState, state);
	}
	if (mState == PLAYER_STOPPED || mState == PLAYER_RUNNING || mState == PLAYER_JOGGING || mState == PLAYER_SPRINTING
			|| mState == PLAYER_TACKLING || mState == PLAYER_SLIDING) {
		if (state == PLAYER_STOPPED_WITH_BALL || state == PLAYER_RUNNING_WITH_BALL || state == PLAYER_JOGGING_WITH_BALL
				|| state == PLAYER_SPRINTING_WITH_BALL) {
			setAnimation(" ", true);
		}
	}
	mState = state;
	switch (mState) {
	case PLAYER_STOPPED:
		setMovementDirection(getMovementDirection());
		setMovementSpeed(0);
		break;
	case PLAYER_RUNNING:
		setMovementDirection(getMovementDirection());
		setMovementSpeed(DEFENDER_MOVEMENT_SPEED);
		break;
	case PLAYER_JOGGING:
		setMovementDirection(getMovementDirection());
		setMovementSpeed(DEFENDER_JOGGING_MOVEMENT_SPEED);
		break;
	case PLAYER_SPRINTING:
		setMovementDirection(getMovementDirection());
		setMovementSpeed(DEFENDER_SPRINTING_SPEED);
		break;
	case PLAYER_STOPPED_WITH_BALL:
		setMovementDirection(getMovementDirection());
		setMovementSpeed(0);
		break;
	case PLAYER_RUNNING_WITH_BALL:
		setMovementDirection(getMovementDirection());
		setMovementSpeed(DEFENDER_WITH_BALL_MOVEMENT_SPEED);
		break;
	case PLAYER_JOGGING_WITH_BALL:
		setMovementDirection(getMovementDirection());
		setMovementSpeed(DEFENDER_WITH_BALL_JOGGING_SPEED);
		break;
	case PLAYER_SPRINTING_WITH_BALL:
		setMovementDirection(getMovementDirection());
		setMovementSpeed(DEFENDER_WITH_BALL_SPRINTING_SPEED);
		break;
	default:
		break;
	}
}

void Defender::ballLost() {

}
