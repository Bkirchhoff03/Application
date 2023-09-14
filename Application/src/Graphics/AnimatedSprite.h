/*
 * AnimatedSprite.h
 *
 *  Created on: Aug 13, 2023
 *      Author: nebk2
 */

#ifndef GRAPHICS_ANIMATEDSPRITE_H_
#define GRAPHICS_ANIMATEDSPRITE_H_

#include "AnimationPlayer.h"
#include "Vec2D.h"
#include "Color.h"
#include <stdint.h>
#include <string>

class AARectangle;
class Screen;
class SpriteSheet;

class AnimatedSprite {
public:
	AnimatedSprite();
	void init(const std::string &animationsPath, const SpriteSheet &spriteSheet, const Color &color = Color::white());
	void update(uint32_t dt);
	void draw(Screen &theScreen);

	void setAnimation(const std::string &animationName, bool looped);
	Vec2D size() const;
	void stop();
	const AARectangle getBoundingBox() const;

	inline Vec2D position() const {
		return mPosition;
	}
	inline void setPosition(const Vec2D &position) {
		mPosition = position;
	}
	inline void moveBy(const Vec2D &delta) {
		mPosition += delta;
	}
	inline bool isFinishedPlayingAnimation() const {
		return mAnimationPlayer.isFinishedPlaying();
	}
	inline const Color& getColor() const {
		return mColor;
	}
	const SpriteSheet* getSpriteSheet() const {
		return mnoptrSpriteSheet;
	}
	
private:
	const SpriteSheet *mnoptrSpriteSheet;
	AnimationPlayer mAnimationPlayer;
	Vec2D mPosition;
	Color mColor;
};


#endif /* GRAPHICS_ANIMATEDSPRITE_H_ */
