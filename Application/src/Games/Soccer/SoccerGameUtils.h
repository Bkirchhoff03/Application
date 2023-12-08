/*
 * SoccerGameUtils.h
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#ifndef GAMES_SOCCER_SOCCERGAMEUTILS_H_
#define GAMES_SOCCER_SOCCERGAMEUTILS_H_

#include "Vec2D.h"
#include <vector>

static const int PLAYER_MOVEMENT_SPEED = 50;
static const int PLAYER_SPRINT_SPEED = 75;
static const int DEFENDER_MOVEMENT_SPEED = 35;
static const int DEFENDER_JOGGING_MOVEMENT_SPEED = 15;
static const int DEFENDER_SPRINTING_SPEED = 55;

static const int PLAYER_WITH_BALL_MOVEMENT_SPEED = 40;
static const int PLAYER_WITH_BALL_SPRINT_SPEED = 65;
static const int DEFENDER_WITH_BALL_MOVEMENT_SPEED = 25;
static const int DEFENDER_WITH_BALL_JOGGING_SPEED = 5;
static const int DEFENDER_WITH_BALL_SPRINTING_SPEED = 45;



enum PlayerMovement {
	PLAYER_MOVEMENT_NONE = 0,
	PLAYER_MOVEMENT_UP,
	PLAYER_MOVEMENT_LEFT,
	PLAYER_MOVEMENT_DOWN,
	PLAYER_MOVEMENT_RIGHT,
	PLAYER_MOVEMENT_LEFT_UP,
	PLAYER_MOVEMENT_LEFT_DOWN,
	PLAYER_MOVEMENT_RIGHT_UP,
	PLAYER_MOVEMENT_RIGHT_DOWN
};

Vec2D getMovementVector(PlayerMovement direction);
PlayerMovement getOppositeDirection(PlayerMovement direction);
std::vector<PlayerMovement> getPerpendicularMovements(PlayerMovement direction);
std::vector<PlayerMovement> getPossibleMovementsBasedOffTarget(Vec2D target, Vec2D location);
std::vector<PlayerMovement> getOtherDirection(PlayerMovement direction);
PlayerMovement getPlayerMovementFromVector(const Vec2D &vec);
std::string millisecondsToMinutesSecondsString(int milliseconds);

#endif /* GAMES_SOCCER_SOCCERGAMEUTILS_H_ */
