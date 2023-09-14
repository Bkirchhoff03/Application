/*
 * Pacman.h
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#ifndef GAMES_PACMAN_PACMAN_H_
#define GAMES_PACMAN_PACMAN_H_

#include "Actor.h"
#include <stdint.h>
class Screen;
class SpriteSheet;

class Pacman: public Actor {
public:

	virtual void init(const SpriteSheet &spriteSheet, const std::string &animationsPath, const Vec2D &intialPos,
			uint32_t movementSpeed, bool updateSpriteOnMovement);
	virtual void update(uint32_t dt) override;
	virtual void setMovementDirection(PacmanMovement movement) override;

	void resetToFirstAnimation();
	void eatenByGhost();
	void resetScore();
	void ateItem(uint32_t value);
	void ateGhost(uint32_t value);

	inline void resetGhostEatenMultiplier() {
		mGhostMultiplier = 1;
	}
	inline uint32_t score() const {
		return mScore;
	}

private:
	void addToScore(uint32_t value);
	uint32_t mScore;
	uint32_t mGhostMultiplier;
	bool mIsDying;
};



#endif /* GAMES_PACMAN_PACMAN_H_ */
