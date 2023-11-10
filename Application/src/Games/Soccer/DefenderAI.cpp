/*
 * DefenderAI.cpp
 *
 *  Created on: Oct 6, 2023
 *      Author: nebk2
 */
#include "DefenderAI.h"
#include "Screen.h"
#include "TeamAgainst.h"
#include "Player.h"
#include <cassert>
#include <algorithm>
#include <vector>
#include "Color.h"
#include "Circle.h"
DefenderAI::DefenderAI() {
}
void DefenderAI::init(Defender &defender, uint32_t lookAheadDistance, const Vec2D &scatterTarget,
		const Vec2D &defenderPositionTarget, const Vec2D &defenderAttackingPosition, DefenderName name) {
	mDefenderZoneTarget = defenderPositionTarget;
	mDefenderExitZonePosition = defenderAttackingPosition;
	mTarget = scatterTarget;
	mLookAheadDistance = lookAheadDistance;
	mnoptrDefender = &defender;
	mName = name;
	std::random_device r;
	mAIRandomGenerator.seed(r());
	mTimer = 0;
	setState(DEFENDER_AI_STATE_START);
	mLastState = DEFENDER_AI_STATE_START;
}

PlayerMovement DefenderAI::update(uint32_t dt, const Player &player, const TeamAgainst &teamAgainst,
		const std::vector<Defender> &defenders) {
	if (mnoptrDefender) {

		if (mState == DEFENDER_AI_STATE_START) {
			return PLAYER_MOVEMENT_NONE;
		}

		if (mState == DEFENDER_AI_STATE_IN_ZONE) {
			mTimer += dt;

			if (mTimer % 1000 < 10) {
				setState(DEFENDER_AI_STATE_EXIT_ZONE);
			}

			return PLAYER_MOVEMENT_NONE;

		}

		if (mState == DEFENDER_AI_STATE_GO_TO_ZONE && mnoptrDefender->position() == mDefenderZoneTarget) {
			setState(DEFENDER_AI_STATE_IN_ZONE);

			mnoptrDefender->setDefenderState(PLAYER_STOPPED);

			return PLAYER_MOVEMENT_NONE;
		}

		if (mState == DEFENDER_AI_STATE_EXIT_ZONE && mnoptrDefender->position() == mDefenderExitZonePosition) {
			setState(DEFENDER_AI_STATE_DEFENDING);
		}

		if (mState == DEFENDER_AI_STATE_DEFENDING) {
			mTimer += dt;
			if (mTimer % 1000 < 5) {
				setState(DEFENDER_AI_STATE_DEFENDING);
			}
		}

		PlayerMovement currentDirection = mnoptrDefender->getMovementDirection();

		std::vector<PlayerMovement> tempDirections;
		std::vector<PlayerMovement> possibleDirections;

		possibleDirections = getPerpendicularMovements(currentDirection);

		if (currentDirection != PLAYER_MOVEMENT_NONE) {
			possibleDirections.push_back(currentDirection);
		}

		for (const PlayerMovement &pDirection : possibleDirections) {
			tempDirections.push_back(pDirection);
		}

		possibleDirections.clear();

		for (const PlayerMovement &direction : tempDirections) {
			if (!teamAgainst.willCollide(*mnoptrDefender, *this, direction)) {
				possibleDirections.push_back(direction);
			}
		}

		//assert(possibleDirections.size() >= 1 && "Why can't we go anywhere?");
		if (possibleDirections.size() == 0) {
			std::cout << mName << " can't go anywhere!" << std::endl;
			std::cout << mState << " is the state" << std::endl;
			assert(false && "Why can't we go anywhere?");
		}

		std::sort(possibleDirections.begin(), possibleDirections.end(),
				[](const PlayerMovement &direction1, const PlayerMovement &direction2) {
					return direction1 < direction2;
				});

		if (mnoptrDefender->isWithBall()
				&& (mState == DEFENDER_AI_STATE_IN_ATTACKING || mState == DEFENDER_AI_STATE_PASSING)) {
			std::uniform_int_distribution<size_t> distribution(0, possibleDirections.size() - 1);
			return possibleDirections[static_cast<int>(distribution(mAIRandomGenerator))];
		}

		if (mState == DEFENDER_AI_STATE_PASSING) {
			changeTarget(getChaseTarget(dt, player, teamAgainst, defenders));
		}

		PlayerMovement directionToGoIn = PLAYER_MOVEMENT_NONE;

		uint32_t lowestDistance = UINT32_MAX;

		for (const PlayerMovement &direction : possibleDirections) {
			Vec2D movementVec = getMovementVector(direction) * mLookAheadDistance;

			AARectangle bbox = mnoptrDefender->getBoundingBox();

			bbox.moveBy(movementVec);

			uint32_t distanceToTarget = bbox.getCenterPoint().distance(mTarget);

			if (distanceToTarget < lowestDistance) {
				directionToGoIn = direction;
				lowestDistance = distanceToTarget;
			}
		}

		assert(directionToGoIn != PLAYER_MOVEMENT_NONE);

		return directionToGoIn;
	}

	return PLAYER_MOVEMENT_NONE;
}
void DefenderAI::draw(Screen &screen) {
}

void DefenderAI::defenderDelegateDefenderStateChangeTo(PlayerState lastState, PlayerState state) {
}
void DefenderAI::defenderWasReleasedFromZone() {
}
void DefenderAI::defenderWasResetToZone() {
}

void DefenderAI::setState(DefenderAIState state) {
}
void DefenderAI::changeTarget(const Vec2D &target) {
}
Vec2D DefenderAI::getChaseTarget(uint32_t dt, const Player &player, const TeamAgainst &teamAgainst,
		const std::vector<Defender> &defender) {
}

