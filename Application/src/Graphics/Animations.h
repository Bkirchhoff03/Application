/*
 * Animations.h
 *
 *  Created on: Aug 11, 2023
 *      Author: nebk2
 */

#ifndef GRAPHICS_ANIMATIONS_H_
#define GRAPHICS_ANIMATIONS_H_

#include "Vec2D.h"
#include "color.h"
#include <string>
#include <vector>

struct AnimationFrame {
	std::string frame = "";
	std::string overlay = "";
	Color frameColor = Color::white();
	Color overlayColor = Color::white();
	Vec2D size;
	Vec2D offset = Vec2D::zero;
	int frameColorSet = 0;
};

class Animation {
public:
	Animation();

	AnimationFrame getAnimationFrame(uint32_t frameNum) const;

	inline void addFrame(const std::string &frame) {
		mFrames.push_back(frame);
	}
	inline void addFrameColor(const Color &color) {
		mFrameColors.push_back(color);
	}
	inline void addOverlayFrameColor(const Color &color) {
		mOverlayColors.push_back(color);
	}
	inline void addFrameOffset(const Vec2D &offset) {
		mFrameOffsets.push_back(offset);
	}

	void setSpriteSheetName(const std::string &spriteSheetName) {
		mSpriteSheetName = spriteSheetName;
	}
	const std::string& getSpriteSheetName() const {
		return mSpriteSheetName;
	}
	void setName(const std::string &animationName) {
		mAnimationName = animationName;
	}
	const std::string& getName() {
		return mAnimationName;
	}

	Vec2D size() const {
		return mSize;
	}

	void setSize(const Vec2D &size) {
		mSize = size;
	}

	void setFPS(int fps) {
		mFPS = fps;
	}
	int FPS() const {
		return mFPS;
	}
	void setOverlay(const std::string &overlayName) {
		mOverlay = overlayName;
	}
	const std::string& overlayName() const {
		return mOverlay;
	}
	size_t numFrames() const {
		return mFrames.size();
	}
	size_t numFramesColors() const {
		return mFrameColors.size();
	}
	size_t numOverlayColors() const {
		return mOverlayColors.size();
	}
	size_t numFramesOffsets() const {
		return mFrameOffsets.size();
	}

	static std::vector<Animation> loadAnimations(const std::string &animationFilePath);
		
private:
	std::string mAnimationName;
	std::string mSpriteSheetName;
	std::string mOverlay;
	std::vector<std::string> mFrames;
	std::vector<Color> mFrameColors;
	std::vector<Color> mOverlayColors;
	std::vector<Vec2D> mFrameOffsets;
	Vec2D mSize;
	int mFPS;

};

#endif /* GRAPHICS_ANIMATIONS_H_ */
