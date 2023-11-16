/*
 * SoccerGameUtils.cpp
 *
 *  Created on: Sep 28, 2023
 *      Author: nebk2
 */

#include "SoccerGameUtils.h"

Vec2D getMovementVector(PlayerMovement direction) {
	if (direction == PLAYER_MOVEMENT_LEFT) {
		return Vec2D(-1, 0);
	} else if (direction == PLAYER_MOVEMENT_DOWN) {
		return Vec2D(0, 1);
	} else if (direction == PLAYER_MOVEMENT_RIGHT) {
		return Vec2D(1, 0);
	} else if (direction == PLAYER_MOVEMENT_UP) {
		return Vec2D(0, -1);
	}
	return Vec2D::zero;
}
PlayerMovement getOppositeDirection(PlayerMovement direction) {
	if (direction == PLAYER_MOVEMENT_LEFT) {
		return PLAYER_MOVEMENT_RIGHT;
	} else if (direction == PLAYER_MOVEMENT_DOWN) {
		return PLAYER_MOVEMENT_UP;
	} else if (direction == PLAYER_MOVEMENT_RIGHT) {
		return PLAYER_MOVEMENT_LEFT;
	} else if (direction == PLAYER_MOVEMENT_UP) {
		return PLAYER_MOVEMENT_DOWN;
	}
	return PLAYER_MOVEMENT_NONE;
}
std::vector<PlayerMovement> getPerpendicularMovements(PlayerMovement direction) {
	std::vector<PlayerMovement> perpendicularDirections;
	if (direction == PLAYER_MOVEMENT_DOWN || direction == PLAYER_MOVEMENT_UP) {
		perpendicularDirections.push_back(PLAYER_MOVEMENT_LEFT);
		perpendicularDirections.push_back(PLAYER_MOVEMENT_RIGHT);
	} else if (direction == PLAYER_MOVEMENT_LEFT || direction == PLAYER_MOVEMENT_RIGHT) {
		perpendicularDirections.push_back(PLAYER_MOVEMENT_UP);
		perpendicularDirections.push_back(PLAYER_MOVEMENT_DOWN);
	} else if (direction == PLAYER_MOVEMENT_NONE) {
		perpendicularDirections.push_back(PLAYER_MOVEMENT_UP);
		perpendicularDirections.push_back(PLAYER_MOVEMENT_LEFT);
		perpendicularDirections.push_back(PLAYER_MOVEMENT_DOWN);
		perpendicularDirections.push_back(PLAYER_MOVEMENT_RIGHT);
	}
	return perpendicularDirections;
}

std::vector<PlayerMovement> getPossibleMovementsBasedOffTarget(Vec2D target, Vec2D location) {
	std::vector<PlayerMovement> possibleMovements;
	if (target.GetX() < location.GetX()) {
		possibleMovements.push_back(PLAYER_MOVEMENT_LEFT);
	}
	else if (target.GetX() > location.GetX()) {
		possibleMovements.push_back(PLAYER_MOVEMENT_RIGHT);
	}
	if (target.GetY() < location.GetY()) {
		possibleMovements.push_back(PLAYER_MOVEMENT_UP);
	}
	else if (target.GetY() > location.GetY()) {
		possibleMovements.push_back(PLAYER_MOVEMENT_DOWN);
	}
	return possibleMovements;
	
}

std::vector<PlayerMovement> getOtherDirection(PlayerMovement direction) {
	std::vector<PlayerMovement> directions;
	for (int dir = PlayerMovement::PLAYER_MOVEMENT_NONE + 1; dir <= PLAYER_MOVEMENT_RIGHT; ++dir) {
		if (dir != direction) {
			directions.push_back(static_cast<PlayerMovement>(dir));
		}
	}
	return directions;
}
