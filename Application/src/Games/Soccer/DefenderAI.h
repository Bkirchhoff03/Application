/*
 * DefenderAI.h
 *
 *  Created on: Sep 22, 2023
 *      Author: nebk2
 */

#ifndef GAMES_SOCCER_DEFENDERAI_H_
#define GAMES_SOCCER_DEFENDERAI_H_


#include "Defender.h"
#include "SoccerGameUtils.h"
#include <random>
#include <stdint.h>
#include <vector>

class TeamAgainst;
class Screen;
class Player;

enum DefenderAIState {
	DEFENDER_AI_STATE_START = 0,
	DEFENDER_AI_STATE_IN_ATTACKING,
	DEFENDER_AI_STATE_IN_ZONE,
	DEFENDER_AI_STATE_EXIT_ZONE,
	DEFENDER_AI_STATE_GO_TO_ZONE,
	DEFENDER_AI_STATE_DEFENDING,
	DEFENDER_AI_STATE_DOUBLE_TEAM,
	DEFENDER_AI_STATE_IS_STEALING,
	DEFENDER_AI_STATE_IS_STEALING_FOUL,
	DEFENDER_AI_STATE_PASSING

};

class DefenderAI: public DefenderDelegate {
public:
	DefenderAI();
	void init(Defender &defender, uint32_t lookAheadDistance, const Vec2D &scatterTarget,
			const Vec2D &defenderPositionTarget, const Vec2D &defenderAttackingPosition, DefenderName name);

	PlayerMovement update(uint32_t dt, const Player &player, const TeamAgainst &teamAgainst,
			const std::vector<Defender> &defenders);
	void draw(Screen &screen);

	inline bool wantsToLeaveZone() const {
		return mState == DEFENDER_AI_STATE_EXIT_ZONE;
	}
	inline bool isInZone() const {
		return mState == DEFENDER_AI_STATE_IN_ZONE || mState == DEFENDER_AI_STATE_START;
	}
	inline bool isEnteringZone() const {
		return mState == DEFENDER_AI_STATE_GO_TO_ZONE;
	}

	virtual void defenderDelegateDefenderStateChangeTo(PlayerState lastState, PlayerState state) override;
	virtual void defenderWasReleasedFromZone() override;
	virtual void defenderWasResetToZone() override;

private:

	void setState(DefenderAIState state);
	void changeTarget(const Vec2D &target);
	Vec2D getChaseTarget(uint32_t dt, const Player &player, const TeamAgainst &teamAgainst,
			const std::vector<Defender> &defender);

	Vec2D mDefenderExitZonePosition;
	Vec2D mDefenderZoneTarget;
	Vec2D mAttackTarget;
	Vec2D mTarget;
	uint32_t mLookAheadDistance;
	Defender *mnoptrDefender;
	DefenderName mName;
	std::default_random_engine mAIRandomGenerator;
	DefenderAIState mState;
	DefenderAIState mLastState;
	uint64_t mTimer;
};



#endif /* GAMES_SOCCER_DEFENDERAI_H_ */
