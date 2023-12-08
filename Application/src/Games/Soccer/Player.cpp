/*
 * Player.cpp
 *
 *  Created on: Sep 25, 2023
 *      Author: nebk2
 */

#include "Player.h"

void Player::init(const SpriteSheet &spriteSheet, const std::string &animationsPath, const Vec2D &intialPos,
		uint32_t movementSpeed, bool updateSpriteOnMovement) {
	SoccerPlayer::init(spriteSheet, animationsPath, intialPos, movementSpeed, false);
	resetToFirstAnimation();
	resetScore();
}
void Player::update(uint32_t dt) {
	SoccerPlayer::update(dt);
}
void Player::setMovementDirection(PlayerMovement movement) {
	PlayerMovement currentDirection = getMovementDirection();

	if (movement == PLAYER_MOVEMENT_LEFT && currentDirection != PLAYER_MOVEMENT_LEFT) {
		setAnimation("move_left", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_RIGHT && currentDirection != PLAYER_MOVEMENT_RIGHT) {
		setAnimation("move_right", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_DOWN && currentDirection != PLAYER_MOVEMENT_DOWN) {
		setAnimation("move_down", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_UP && currentDirection != PLAYER_MOVEMENT_UP) {
		setAnimation("move_up", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_LEFT_UP && currentDirection != PLAYER_MOVEMENT_LEFT_UP) {
		setAnimation("move_up_left", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_LEFT_DOWN && currentDirection != PLAYER_MOVEMENT_LEFT_DOWN) {
		setAnimation("move_down_left", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_RIGHT_UP && currentDirection != PLAYER_MOVEMENT_RIGHT_UP) {
		setAnimation("move_up_right", true);
		resetDelta();
	} else if (movement == PLAYER_MOVEMENT_RIGHT_DOWN && currentDirection != PLAYER_MOVEMENT_RIGHT_DOWN) {
		setAnimation("move_down_right", true);
		resetDelta();
	}
	setMovementSpeed(50);
	SoccerPlayer::setMovementDirection(movement);
}

void Player::resetToFirstAnimation() {
}
void Player::ballTakenByDefender() {
}
void Player::resetScore() {
	mScore = 0;
}

void Player::addToScore(uint32_t value) {
	mScore += value;
}

