/*
 * Actor.h
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#ifndef GAMES_PACMAN_ACTOR_H_
#define GAMES_PACMAN_ACTOR_H_

#include "AnimatedSprite.h"
#include "Vec2D.h"
#include "PacmanGameUtils.h"
#include "AARectangle.h"
#include "Color.h"
#include <string>
#include <stdint.h>

class Screen;
class SpriteSheet;

class Actor {
public:
	virtual ~Actor() {
	}

	virtual void init(const SpriteSheet &spriteSheet, const std::string &animationsPath, const Vec2D &intialPos,
			uint32_t movementSpeed, bool updateSpriteOnMovement, const Color &spriteColor = Color::white());
	virtual void update(uint32_t dt);
	virtual void draw(Screen &screen);

	virtual void stop();
	AARectangle getEatingBoundingBox() const;

	inline bool isFinishedAnimation() const {
		return mSprite.isFinishedPlayingAnimation();
	}
	inline const AARectangle getBoundingBox() const {
		return mSprite.getBoundingBox();
	}
	inline void moveBy(const Vec2D &delta) {
		mSprite.moveBy(delta);
	}
	inline void moveTo(const Vec2D &position) {
		mSprite.setPosition(position);
	}
	inline Vec2D position() {
		return mSprite.position();
	}
	inline PacmanMovement getMovementDirection() const {
		return mMovementDirection;
	}
	virtual inline void setMovementDirection(PacmanMovement direction) {
		mMovementDirection = direction;
	}
	inline const Color& getSpriteColor() const {
		return mSprite.getColor();
	}
	
protected:
	void setAnimation(const std::string &animationName, bool looped);

	inline void resetDelta() {
		mDelta = Vec2D::zero;
	}
	inline void setMovementSpeed(uint32_t movementSpeed) {
		mMovementSpeed = movementSpeed;
	}

	AnimatedSprite mSprite;
private:
	Vec2D mDelta;
	PacmanMovement mMovementDirection;
	uint32_t mMovementSpeed;
	bool mUpdateSpriteOnUpdate;
};

#endif /* GAMES_PACMAN_ACTOR_H_ */
