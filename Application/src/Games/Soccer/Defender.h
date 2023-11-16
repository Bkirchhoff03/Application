/*
 * Defender.h
 *
 *  Created on: Sep 22, 2023
 *      Author: nebk2
 */

#ifndef GAMES_SOCCER_DEFENDER_H_
#define GAMES_SOCCER_DEFENDER_H_

#include "SoccerPlayer.h"
#include "Color.h"
#include "SoccerGameUtils.h"
#include "AARectangle.h"

enum DefenderName {
	CENTER_BACK = 0, LEFT_BACK, RIGHT_BACK, GOALKEEPER, CENTER_DEFENSIVE_MIDFIELDER, NUM_DEFENDERS
};

class DefenderDelegate {
public:
	virtual ~DefenderDelegate() {
	}
	virtual void defenderDelegateDefenderStateChangeTo(PlayerState lastState, PlayerState state) = 0;
	virtual void defenderWasReleasedFromZone() = 0;
	virtual void defenderWasResetToZone() = 0;
};

class Defender: public SoccerPlayer {
public:

	Defender();
	virtual void init(const SpriteSheet &spriteSheet, const std::string &animationsPath, const Vec2D &intialPos,
			uint32_t movementSpeed, bool updateSpriteOnMovement, const Color &spriteColor = Color::white()) override;
	virtual void update(uint32_t dt) override;

	virtual void setMovementDirection(PlayerMovement direction) override;
	virtual void stop() override;

	void resetToFirstPosition();
	void setZone(const AARectangle rect);
	inline bool isSliding() const {
		return mState == PLAYER_SLIDING;
	}
	inline bool isWithBall() const {
		return mState == PLAYER_RUNNING_WITH_BALL || mState == PLAYER_JOGGING_WITH_BALL
				|| mState == PLAYER_SPRINTING_WITH_BALL;
	}
	inline bool isWithoutBall() const {
		return !isWithBall();
	}
	inline void lockCanChangeDirection() {
		mCanChangeDirection = false;
	}
	inline bool canChangeDirection() const {
		return mCanChangeDirection;
	}
	inline const AARectangle zoneBoundingBox() const {
		return zoneBBox;
	}
	void scoredOnByPlayer();

	void setDefenderDelegate(DefenderDelegate &delegate) {
		mDelegate = &delegate;
	}
	inline bool isRealeasedFromZone() const {
		return !mIsInZone;
	}
	void setStateToDefending();
	void releaseFromZone();
	inline const uint32_t getTackleOdds() const {
		return mTackleOdds;
	}
	inline const uint32_t getFoulOdds() const {
		return mFoulOdds;
	}

	void ballLost();

private:
	void setDefenderState(PlayerState state);
	friend class DefenderAI;
	PlayerState mState;
	bool mCanChangeDirection;
	Vec2D mInitialPos;
	AARectangle zoneBBox;
	bool mIsInZone;
	DefenderDelegate *mDelegate;
	uint32_t mTackleOdds;
	uint32_t mFoulOdds;
};

#endif /* GAMES_SOCCER_DEFENDER_H_ */
