/*
 * AnimatedSprite.cpp
 *
 *  Created on: Aug 13, 2023
 *      Author: nebk2
 */

#include "AnimatedSprite.h"
#include "AARectangle.h"
#include "Screen.h"

AnimatedSprite::AnimatedSprite() :
		mPosition(Vec2D::zero), mnoptrSpriteSheet(nullptr) {
}
void AnimatedSprite::init(const std::string &animationsPath, const SpriteSheet &spriteSheet, const Color &color) {
	mAnimationPlayer.init(animationsPath);
	mnoptrSpriteSheet = &spriteSheet;
	mColor = color;
}
void AnimatedSprite::update(uint32_t dt) {
	mAnimationPlayer.update(dt);
}
void AnimatedSprite::draw(Screen &theScreen) {
	AnimationFrame frame = mAnimationPlayer.getCurrentAnimationFrame();
	Color frameColor = frame.frameColor;
	if (!frame.frameColorSet) {
		frameColor = mColor;
	}
	theScreen.draw(*mnoptrSpriteSheet, frame.frame, mPosition + frame.offset, frameColor);
	if (frame.overlay.size() > 0) {
		theScreen.draw(*mnoptrSpriteSheet, frame.overlay, mPosition, frame.overlayColor);
	}
}

void AnimatedSprite::setAnimation(const std::string &animationName, bool looped) {
	mAnimationPlayer.play(animationName, looped);
}
Vec2D AnimatedSprite::size() const {
	return mAnimationPlayer.getCurrentAnimationFrame().size;
}
void AnimatedSprite::stop() {
	mAnimationPlayer.stop();
}
const AARectangle AnimatedSprite::getBoundingBox() const {
	AARectangle bbox = { mPosition, static_cast<unsigned int>(size().GetX()), static_cast<unsigned int>(size().GetY()) };
	return bbox;
}

