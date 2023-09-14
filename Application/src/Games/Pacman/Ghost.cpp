/*
 * Ghost.cpp
 *
 *  Created on: Aug 24, 2023
 *      Author: nebk2
 */

#include "Ghost.h"
namespace {
const uint32_t NUM_POINTS_FOR_GHOST = 200;
}
Ghost::Ghost() :
		mPoints(0), mInitialPos(Vec2D::zero), mIsReleased(false), mDelegate(nullptr) {

}

void Ghost::init(const SpriteSheet &spriteSheet, const std::string &animationsPath, const Vec2D &intialPos,
		uint32_t movementSpeed, bool updateSpriteOnMovement, const Color &spriteColor) {
	Actor::init(spriteSheet, animationsPath, intialPos, movementSpeed, updateSpriteOnMovement, spriteColor);
	mInitialPos = intialPos;
	mPoints = NUM_POINTS_FOR_GHOST;
	resetToFirstPosition();
}
void Ghost::update(uint32_t dt) {
	Vec2D pos = position();
	Actor::update(dt);
	mCanChangeDirection = pos != position();
	if (isVulnerable()) {
		mVulnerablityTimer += dt;
		if (mState == GHOST_STATE_VULNERABLE && mVulnerablityTimer >= VULNERABILITY_TIME) {
			setGhostState(GHOST_STATE_VULNERABLE_ENDING);
		}
		if (mState == GHOST_STATE_VULNERABLE_ENDING && mVulnerablityTimer >= VULNERABILITY_ENDING_TIME) {
			setGhostState(GHOST_STATE_ALIVE);
		}
	}
}

void Ghost::setStateToVulnerable() {
	if (mState != GHOST_STATE_DEAD) {
		setGhostState(GHOST_STATE_VULNERABLE);
	}
}
void Ghost::setMovementDirection(PacmanMovement direction) {
	Actor::setMovementDirection(direction);
	PacmanMovement movementDir = getMovementDirection();

	if (mState == GHOST_STATE_ALIVE) {
		if (movementDir == PACMAN_MOVEMENT_RIGHT) {
			setAnimation("ghost_movement_right", true);
		} else if (movementDir == PACMAN_MOVEMENT_UP) {
			setAnimation("ghost_movement_up", true);
		} else if (movementDir == PACMAN_MOVEMENT_DOWN) {
			setAnimation("ghost_movement_down", true);
		} else if (movementDir == PACMAN_MOVEMENT_LEFT) {
			setAnimation("ghost_movement_left", true);
		}
	} else if (mState == GHOST_STATE_DEAD) {
		if (movementDir == PACMAN_MOVEMENT_RIGHT) {
			setAnimation("ghost_dead_right", true);
		} else if (movementDir == PACMAN_MOVEMENT_UP) {
			setAnimation("ghost_dead_up", true);
		} else if (movementDir == PACMAN_MOVEMENT_DOWN) {
			setAnimation("ghost_dead_down", true);
		} else if (movementDir == PACMAN_MOVEMENT_LEFT) {
			setAnimation("ghost_dead_left", true);
		}
	}
}
void Ghost::stop() {
	setMovementDirection(PACMAN_MOVEMENT_NONE);
}

void Ghost::eatenByPacman() {
	setGhostState(GHOST_STATE_DEAD);
}
void Ghost::resetToFirstPosition() {
	mSprite.setPosition(mInitialPos);
	setMovementDirection(PACMAN_MOVEMENT_NONE);
	mVulnerablityTimer = 0;
	setGhostState(GHOST_STATE_ALIVE);
	mCanChangeDirection = true;
	mIsReleased = false;
	if (mDelegate) {
		mDelegate->ghostWasResetToFirstPosition();
	}
	
}

void Ghost::setGhostState(GhostState state) {
	if (mDelegate) {
		mDelegate->ghostDelegateGhostStateChangeTo(mState, state);
	}

	mState = state;
	switch (mState) {
	case GHOST_STATE_ALIVE:
		setMovementDirection(getMovementDirection());
		setMovementSpeed(GHOST_MOVEMENT_SPEED);
		break;
	case GHOST_STATE_VULNERABLE:
		setAnimation("ghost_vulnerable", true);
		mVulnerablityTimer = 0;
		setMovementSpeed(GHOST_VULNERABLE_MOVEMENT_SPEED);
		break;
	case GHOST_STATE_VULNERABLE_ENDING:
		setAnimation("ghost_vulnerable_ending", true);
		mVulnerablityTimer = 0;
		break;
	case GHOST_STATE_DEAD:
		setMovementDirection(getMovementDirection());
		setMovementSpeed(GHOST_BACK_TO_PEN_SPEED);
		break;

	default:
		break;
	}
}

void Ghost::releasedFromPen() {
	mIsReleased = true;
	if (mDelegate) {
		mDelegate->ghostWasReleasedFromPen();
	}
}
