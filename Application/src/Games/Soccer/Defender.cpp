/*
 * Defender.cpp
 *
 *  Created on: Sep 25, 2023
 *      Author: nebk2
 */

#include "Defender.h"

Defender::Defender() :
	mDelegate(nullptr), mCanChangeDirection(true), mState(PLAYER_STOPPED), mIsInZone(true) {

}
void Defender::init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& intialPos,
	uint32_t movementSpeed, bool updateSpriteOnMovement, const Color& spriteColor) {
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
	setMovementDirection(PLAYER_MOVEMENT_LEFT);
	mState = PLAYER_STOPPED;
	setDefenderState(PLAYER_STOPPED);
	setAnimation("defender_movement_left", true);
	mCanChangeDirection = true;
	if (mDelegate) {
		mDelegate->defenderWasResetToZone();
	}

}

void Defender::checkIntersection(Player& player) {
	if (player.getBoundingBox().intersects(getBoundingBox())) {
		AARectangle playerBBox = player.getBoundingBox();
		AARectangle defenderBBox = getBoundingBox();
		AARectangle intersection = AARectangle::overlappingRectangle(playerBBox, defenderBBox);
		//std::cout << "intersection: " << intersection.getWidth() << ", " << intersection.getHeight() << std::endl;
		if (intersection.getWidth() > intersection.getHeight()) {
			//top or bottom collision
			if (playerBBox.getCenterPoint().GetY() < defenderBBox.getCenterPoint().GetY()) {
				//bottom collision
				moveBy(Vec2D(0, intersection.getHeight()));
			}
			else {
				//top collision
				moveBy(Vec2D(0, -intersection.getHeight()));
			}
		}
		else {
			//left or right collision
			if (playerBBox.getCenterPoint().GetX() < defenderBBox.getCenterPoint().GetX()) {
				//right collision
				moveBy(Vec2D(intersection.getWidth(), 0));
			}
			else {
				//left collision
				moveBy(Vec2D(-intersection.getWidth(), 0));
			}
		}
		setToDefended();
		player.setToDefended();
	}
	else {
		setToNotDefended();
		player.setToNotDefended();
	}
}

void Defender::setZone(const AARectangle rect) {
	zoneBBox = rect;
}

void Defender::setMovementDirection(PlayerMovement direction) {
	//SoccerPlayer::setMovementDirection(direction);
	PlayerMovement movementDir = getMovementDirection();

	if (mState == PLAYER_STOPPED || mState == PLAYER_SPRINTING || mState == PLAYER_RUNNING || mState == PLAYER_JOGGING
		|| mState == PLAYER_SLIDING || mState == PLAYER_TACKLING) {
		if (direction == PLAYER_MOVEMENT_RIGHT && movementDir != PLAYER_MOVEMENT_RIGHT) {
			setAnimation("move_right", true);
			resetDelta();
		}
		else if (direction == PLAYER_MOVEMENT_UP && movementDir != PLAYER_MOVEMENT_UP) {
			setAnimation("move_up", true);
			resetDelta();
		}
		else if (direction == PLAYER_MOVEMENT_DOWN && movementDir != PLAYER_MOVEMENT_DOWN) {
			setAnimation("move_down", true);
			resetDelta();
		}
		else if (direction == PLAYER_MOVEMENT_LEFT && movementDir != PLAYER_MOVEMENT_LEFT) {
			setAnimation("move_left", true);
			resetDelta();
		}
		else if (direction == PLAYER_MOVEMENT_LEFT_UP && movementDir != PLAYER_MOVEMENT_LEFT_UP) {
			setAnimation("move_left_up", true);
			resetDelta();
		}
		else if (direction == PLAYER_MOVEMENT_LEFT_DOWN && movementDir != PLAYER_MOVEMENT_LEFT_DOWN) {
			setAnimation("move_left_down", true);
			resetDelta();
		}
		else if (direction == PLAYER_MOVEMENT_RIGHT_UP && movementDir != PLAYER_MOVEMENT_RIGHT_UP) {
			setAnimation("move_right_up", true);
			resetDelta();
		}
		else if (direction == PLAYER_MOVEMENT_RIGHT_DOWN && movementDir != PLAYER_MOVEMENT_RIGHT_DOWN) {
			setAnimation("move_right_down", true);
			resetDelta();
		}
	}/*
	else if (mState == PLAYER_STOPPED_WITH_BALL || mState == PLAYER_SPRINTING_WITH_BALL
		|| mState == PLAYER_RUNNING_WITH_BALL || mState == PLAYER_JOGGING_WITH_BALL) {
		if (movementDir == PLAYER_MOVEMENT_RIGHT) {
			setAnimation("defender_movement_with_ball_right", true);
		}
		else if (movementDir == PLAYER_MOVEMENT_UP) {
			setAnimation("defender_movement_with_ball_up", true);
		}
		else if (movementDir == PLAYER_MOVEMENT_DOWN) {
			setAnimation("defender_movement_with_ball_down", true);
		}
		else if (movementDir == PLAYER_MOVEMENT_LEFT&&) {
			setAnimation("defender_movement_with_ball_left", true);
		}
	}*/
	SoccerPlayer::setMovementDirection(direction);
}
void Defender::stop() {
	setMovementDirection(PLAYER_MOVEMENT_NONE);
}

void Defender::setStateToDefending() {
	if (!isWithBall()) {
		setDefenderState(PLAYER_SPRINTING);

	}
}

void Defender::setStateToReturnZone() {
	setDefenderState(PLAYER_JOGGING);
}

void Defender::setStateToInZone() {
	setDefenderState(PLAYER_STOPPED);
}

void Defender::setDefenderState(PlayerState state) {
	if (mDelegate) {
		mDelegate->defenderDelegateDefenderStateChangeTo(mState, state);
	}
	//if (mState == PLAYER_STOPPED || mState == PLAYER_RUNNING || mState == PLAYER_JOGGING || mState == PLAYER_SPRINTING
	//		|| mState == PLAYER_TACKLING || mState == PLAYER_SLIDING) {
	//	if (state == PLAYER_STOPPED_WITH_BALL || state == PLAYER_RUNNING_WITH_BALL || state == PLAYER_JOGGING_WITH_BALL
	//			|| state == PLAYER_SPRINTING_WITH_BALL) {
	//		setAnimation("defender_movement_with_ball_down", true);
	//	}
	//}
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

void Defender::releaseFromZone() {
	if (mIsInZone) {
		mIsInZone = false;
	}
}
