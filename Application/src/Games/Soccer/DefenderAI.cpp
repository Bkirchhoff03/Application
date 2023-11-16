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
		mTimer += dt;
		if (mTimer > 1000) {
			mTimer = 0;
		}
		if (mState == DEFENDER_AI_STATE_START) {
			setState(DEFENDER_AI_STATE_IN_ZONE);
		}
		if (mState == DEFENDER_AI_STATE_DOUBLE_TEAM) {
			if (player.isWithoutBall()) {
				setState(DEFENDER_AI_STATE_GO_TO_ZONE);
			}
		}
		if (mState == DEFENDER_AI_STATE_IN_ZONE) {
			
			std::cout << "Zone size " << mnoptrDefender->zoneBoundingBox().getWidth() << " " << mnoptrDefender->zoneBoundingBox().getHeight() << std::endl;
			std::cout << "Player size " << player.getBoundingBox().getWidth() << " " << player.getBoundingBox().getHeight() << std::endl;
			std::cout << "colliding with zone " << mnoptrDefender->zoneBoundingBox().intersects(player.getBoundingBox()) << std::endl;
			if (mnoptrDefender->zoneBoundingBox().intersects(player.getBoundingBox())) {

				changeTarget(player.getBoundingBox().getCenterPoint());
				setState(DEFENDER_AI_STATE_DEFENDING);
				mnoptrDefender->setDefenderState(PLAYER_JOGGING);
			} else {
				//return PLAYER_MOVEMENT_NONE;
			}
		}

		if (mState == DEFENDER_AI_STATE_GO_TO_ZONE && mnoptrDefender->getBoundingBox().intersects(zone)) {
			setState(DEFENDER_AI_STATE_IN_ZONE);
			mnoptrDefender->setDefenderState(PLAYER_STOPPED);
			//return PLAYER_MOVEMENT_NONE;
		}

		if (mState == DEFENDER_AI_STATE_EXIT_ZONE && !mnoptrDefender->getBoundingBox().intersects(zone)) {
			setState(DEFENDER_AI_STATE_DOUBLE_TEAM);
			mnoptrDefender->setDefenderState(PLAYER_JOGGING);
		}

		if (mState == DEFENDER_AI_STATE_DEFENDING) {
			if (player.getBoundingBox().intersects(zone)) {
				setState(DEFENDER_AI_STATE_DEFENDING);
				mnoptrDefender->setDefenderState(PLAYER_JOGGING);
			}
		}

		PlayerMovement currentDirection = mnoptrDefender->getMovementDirection();

		std::vector<PlayerMovement> tempDirections;
		std::vector<PlayerMovement> possibleDirections;
		std::cout << mState << " is the state" << std::endl;
		std::cout << mTarget.GetX() << " " << mTarget.GetY() << " is the target" << std::endl;
		std::cout << mnoptrDefender->getMovementDirection() << " is the movement direction" << std::endl;

		possibleDirections = getPossibleMovementsBasedOffTarget(mTarget, mnoptrDefender->position());

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

void DefenderAI::setZone(const AARectangle rect) {
	zone = rect;
	mnoptrDefender->setZone(rect);
}
void DefenderAI::draw(Screen &screen) {
	if (mnoptrDefender) {
		Circle targetCircle = Circle(mTarget, 4);

		screen.draw(targetCircle, mnoptrDefender->getSpriteColor(), true, mnoptrDefender->getSpriteColor());

		AARectangle bbox = zone;

		/*bbox.moveBy(
				getMovementVector(mnoptrDefender->getMovementDirection())
						* mnoptrDefender->getBoundingBox().getWidth());*/

		Color c = Color(mnoptrDefender->getSpriteColor().getRed(), mnoptrDefender->getSpriteColor().getGreen(),
				mnoptrDefender->getSpriteColor().getBlue(), 200);
		screen.draw(bbox, mnoptrDefender->getSpriteColor(), true, c);
	}
}

void DefenderAI::defenderDelegateDefenderStateChangeTo(PlayerState lastState, PlayerState state) {
	if (mnoptrDefender && mnoptrDefender->isRealeasedFromZone() && !(isInZone() || wantsToLeaveZone())) {
		mnoptrDefender->setMovementDirection(getOppositeDirection(mnoptrDefender->getMovementDirection()));
	}
	if (lastState == PLAYER_STOPPED && state == PLAYER_JOGGING) {
		setState(DEFENDER_AI_STATE_DEFENDING);
	}
	if (state == PLAYER_STOPPED) {
		setState(DEFENDER_AI_STATE_GO_TO_ZONE);
	} else if ((lastState == PLAYER_JOGGING || lastState == PLAYER_RUNNING) && state == PLAYER_SPRINTING) {
		if (mState == DEFENDER_AI_STATE_DEFENDING || mState == DEFENDER_AI_STATE_IN_ZONE) {
			setState(mLastState);
		}
	}
}
void DefenderAI::defenderWasReleasedFromZone() {
	if (mState == DEFENDER_AI_STATE_START) {
		setState(DEFENDER_AI_STATE_IN_ZONE);
	}
}
void DefenderAI::defenderWasResetToZone() {
	setState(DEFENDER_AI_STATE_START);
}

void DefenderAI::setState(DefenderAIState state) {
	if (mState == DEFENDER_AI_STATE_IN_ZONE || mState == DEFENDER_AI_STATE_DEFENDING) {
		mLastState = mState;
	}
	mState = state;

	switch (state) {
	case DEFENDER_AI_STATE_IN_ZONE:
		break;
	case DEFENDER_AI_STATE_GO_TO_ZONE: {
		Vec2D target = { mDefenderZoneTarget.GetX() + mnoptrDefender->getBoundingBox().getWidth() / 2,
				mDefenderZoneTarget.GetY() - mnoptrDefender->getBoundingBox().getHeight() / 2 };
		changeTarget(target);
	}
		break;
	case DEFENDER_AI_STATE_EXIT_ZONE:
		changeTarget(mDefenderExitZonePosition);
		break;
	case DEFENDER_AI_STATE_DEFENDING:
		changeTarget(mTarget);
		break;
	case DEFENDER_AI_STATE_DOUBLE_TEAM:
		changeTarget(mTarget);
		break;
	default:
		break;
	}

}
void DefenderAI::changeTarget(const Vec2D &target) {
	mTarget = target;
}
Vec2D DefenderAI::getChaseTarget(uint32_t dt, const Player &player, const TeamAgainst &teamAgainst,
		const std::vector<Defender> &defenders) {
	Vec2D target;
	switch (mName) {
	case GOALKEEPER: {
		target = player.getBoundingBox().getCenterPoint();
	}
		break;
	case CENTER_BACK: {
		target = player.getBoundingBox().getCenterPoint()
				+ 2 * getMovementVector(player.getMovementDirection()) * player.getBoundingBox().getWidth();
	}
		break;
	case LEFT_BACK: {
		Vec2D pacmanOffsetPoint = player.getBoundingBox().getCenterPoint()
				+ (getMovementVector(player.getMovementDirection()) * player.getBoundingBox().getWidth());
		target = (pacmanOffsetPoint - defenders[CENTER_DEFENSIVE_MIDFIELDER].getBoundingBox().getCenterPoint()) * 2
				+ defenders[CENTER_DEFENSIVE_MIDFIELDER].getBoundingBox().getCenterPoint();
	}
		break;
	case RIGHT_BACK: {
		auto distanceToPacmanBox = mnoptrDefender->getBoundingBox().getCenterPoint().distance(
				player.getBoundingBox().getCenterPoint());
		if (distanceToPacmanBox > player.getBoundingBox().getWidth() * 4) {
			target = player.getBoundingBox().getCenterPoint();
		} else {
			target = mAttackTarget;
		}
	}
		break;
	case CENTER_DEFENSIVE_MIDFIELDER: {
		target = player.getBoundingBox().getCenterPoint();
	}
		break;
	default: {
	}
		break;
	}
	return target;
}

