/*
 * AnimationPlayer.cpp
 *
 *  Created on: Aug 12, 2023
 *      Author: nebk2
 */

#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer() :
		mTime(0), mCurrentAnimation(0), mFrame(0), mLooped(false), mIsPlaying(false), mIsFinishedPlaying(false) {

}
bool AnimationPlayer::init(const std::string &animationsPath) {
	mAnimations = Animation::loadAnimations(animationsPath);
	return mAnimations.size() > 0;
}
bool AnimationPlayer::play(const std::string &animationName, bool looped) {
	bool found = false;

	for (size_t i = 0; i < mAnimations.size(); ++i) {
		if (mAnimations[i].getName() == animationName) {
			mCurrentAnimation = i;
			found = true;
			break;
		}
	}
	if (found) {
		mIsPlaying = true;
		mFrame = 0;
		mTime = 0;
		mLooped = looped;
		mIsFinishedPlaying = false;
	}
	return found;
}
void AnimationPlayer::pause() {
	if (mIsFinishedPlaying) {
		mIsPlaying = !mIsPlaying;
	}
}
void AnimationPlayer::stop() {
	mIsPlaying = false;
	mTime = 0;
	mFrame = 0;
	mIsFinishedPlaying = true;
}
void AnimationPlayer::update(uint32_t dt) {
	if (mIsPlaying) {
		mTime += dt;

		int fps = mAnimations[mCurrentAnimation].FPS();

		float millisecondsPerFrame = static_cast<float>(1000) / static_cast<float>(fps);

		if (static_cast<float>(mTime) > millisecondsPerFrame) {
			mTime -= static_cast<uint32_t>(millisecondsPerFrame);

			if (mFrame + 1 < mAnimations[mCurrentAnimation].numFrames()) {
				++mFrame;
			} else if ((mFrame + 1 >= mAnimations[mCurrentAnimation].numFrames()) && mLooped) {
				mFrame = 0;
			} else {
				mIsFinishedPlaying = true;
			}
		}
	}
}
AnimationFrame AnimationPlayer::getCurrentAnimationFrame() const {
	if (mAnimations.empty()) {
		return AnimationFrame();
	}
	return mAnimations[mCurrentAnimation].getAnimationFrame(getCurrentFrameNumber());
}

