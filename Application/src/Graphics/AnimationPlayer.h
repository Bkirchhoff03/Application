/*
 * AnimationPlayer.h
 *
 *  Created on: Aug 12, 2023
 *      Author: nebk2
 */

#ifndef GRAPHICS_ANIMATIONPLAYER_H_
#define GRAPHICS_ANIMATIONPLAYER_H_

#include "Animations.h"
#include <stdint.h>
#include <vector>
#include <string>

class AnimationPlayer {
public:
	AnimationPlayer();
	bool init(const std::string &animationsPath);
	bool play(const std::string &animationName, bool looped);
	void pause();
	void stop();
	void update(uint32_t dt);
	AnimationFrame getCurrentAnimationFrame() const;

	inline bool isFinishedPlaying() const {
		return mIsFinishedPlaying;
	}
	inline uint32_t getCurrentFrameNumber() const {
		return mFrame;
	}
private:
	uint32_t mTime;
	std::vector<Animation> mAnimations;
	size_t mCurrentAnimation;
	uint32_t mFrame;

	bool mLooped;
	bool mIsPlaying;
	bool mIsFinishedPlaying;

};



#endif /* GRAPHICS_ANIMATIONPLAYER_H_ */
