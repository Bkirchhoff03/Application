/*
 * SoccerPlayer.h
 *
 *  Created on: Sep 22, 2023
 *      Author: nebk2
 */

#ifndef GAMES_SOCCER_SOCCERPLAYER_H_
#define GAMES_SOCCER_SOCCERPLAYER_H_

#include "AnimatedSprite.h"
#include "Vec2D.h"
#include "SoccerGameUtils.h"
#include "AARectangle.h"
#include "Color.h"
#include <string>
#include <stdint.h>

class Screen;
class SpriteSheet;

enum PlayerState {
	PLAYER_STOPPED = 0,
	PLAYER_RUNNING,
	PLAYER_JOGGING,
	PLAYER_SPRINTING,
	PLAYER_TACKLING,
	PLAYER_SLIDING,
	PLAYER_STOPPED_WITH_BALL,
	PLAYER_RUNNING_WITH_BALL,
	PLAYER_JOGGING_WITH_BALL,
	PLAYER_SPRINTING_WITH_BALL,
};

class SoccerPlayer {
public:
	virtual ~SoccerPlayer() {
	}

	virtual void init(const SpriteSheet &spriteSheet, const std::string &animationsPath, const Vec2D &intialPos,
			uint32_t movementSpeed, bool updateSpriteOnMovement, const Color &spriteColor = Color::white());
	virtual void update(uint32_t dt);
	virtual void draw(Screen &screen);

	virtual void stop();
	AARectangle getDribbleBoundingBox() const;

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
	inline PlayerMovement getMovementDirection() const {
		return mMovementDirection;
	}
	virtual inline void setMovementDirection(PlayerMovement direction) {
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
	PlayerMovement mMovementDirection;
	uint32_t mMovementSpeed;
	bool mUpdateSpriteOnUpdate;
};

#endif /* GAMES_SOCCER_SOCCERPLAYER_H_ */
