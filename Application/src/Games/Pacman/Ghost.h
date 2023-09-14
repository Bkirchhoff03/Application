/*
 * Ghost.h
 *
 *  Created on: Aug 18, 2023
 *      Author: nebk2
 */

#ifndef GAMES_PACMAN_GHOST_H_
#define GAMES_PACMAN_GHOST_H_

#include "Actor.h"
#include "Color.h"
#include "PacmanGameUtils.h"
#include "AARectangle.h"

enum GhostName {
	BLINKY = 0, PINKY, INKY, CLYDE, NUM_GHOSTS
};

enum GhostState {
	GHOST_STATE_ALIVE = 0, GHOST_STATE_VULNERABLE, GHOST_STATE_VULNERABLE_ENDING, GHOST_STATE_DEAD
};

class GhostDelegate {
public:
	virtual ~GhostDelegate() {
	}
	virtual void ghostDelegateGhostStateChangeTo(GhostState lastState, GhostState state) = 0;
	virtual void ghostWasReleasedFromPen() = 0;
	virtual void ghostWasResetToFirstPosition() = 0;
};

class Ghost: public Actor {
public:
	static const uint32_t VULNERABILITY_TIME = 6000;
	static const uint32_t VULNERABILITY_ENDING_TIME = 4000;

	Ghost();
	virtual void init(const SpriteSheet &spriteSheet, const std::string &animationsPath, const Vec2D &intialPos,
			uint32_t movementSpeed, bool updateSpriteOnMovement, const Color &spriteColor = Color::white()) override;
	virtual void update(uint32_t dt) override;

	void setStateToVulnerable();
	virtual void setMovementDirection(PacmanMovement direction) override;
	virtual void stop() override;

	void eatenByPacman();
	void resetToFirstPosition();

	void releasedFromPen();
	inline bool isRealeased() const {
		return mIsReleased;
	}
	
	inline bool isDead() const {
		return mState == GHOST_STATE_DEAD;
	}
	inline bool isVulnerable() const {
		return mState == GHOST_STATE_VULNERABLE || mState == GHOST_STATE_VULNERABLE_ENDING;
	}
	inline bool isAlive() const {
		return mState == GHOST_STATE_ALIVE;
	}
	inline uint32_t getPoints() const {
		return mPoints;
	}
	inline void lockCanChangeDirection() {
		mCanChangeDirection = false;
	}
	inline bool canChangeDirection() const {
		return mCanChangeDirection;
	}
		
	void setGhostDelegate(GhostDelegate &delegate) {
		mDelegate = &delegate;
	}

private:
	void setGhostState(GhostState state);

	friend class GhostAI;
	uint32_t mVulnerablityTimer;
	uint32_t mPoints;
	GhostState mState;
	bool mCanChangeDirection;
	Vec2D mInitialPos;
	bool mIsReleased;
	GhostDelegate *mDelegate;
};

#endif /* GAMES_PACMAN_GHOST_H_ */
