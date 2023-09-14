/*
 * Animation.cpp
 *
 *  Created on: Aug 12, 2023
 *      Author: nebk2
 */

#include "Animations.h"
#include "FileCommandLoader.h"
#include <cassert>

Animation::Animation() :
		mSize(Vec2D::zero), mSpriteSheetName(""), mAnimationName(""), mFPS(0) {

}
AnimationFrame Animation::getAnimationFrame(uint32_t frameNum) const {
	AnimationFrame frame;
	if (frameNum > mFrames.size()) {
		return frame;
	}
	frame.frame = mFrames[frameNum];
	if (frameNum < mFrameColors.size()) {
		frame.frameColor = mFrameColors[frameNum];
		frame.frameColorSet = 1;
	}
	if (mOverlay.size() > 0) {
		frame.overlay = mOverlay;
	}
	if (frameNum < mOverlayColors.size()) {
		frame.overlayColor = mOverlayColors[frameNum];
	}
	if (frameNum < mFrameOffsets.size()) {
		frame.offset = mFrameOffsets[frameNum];
	}
	frame.size = mSize;
	return frame;
}

std::vector<Animation> Animation::loadAnimations(const std::string &animationFilePath) {
	std::vector<Animation> animations;
	FileCommandLoader fileLoader;
	Command animationCommand;
	animationCommand.command = "animation";
	animationCommand.parseFunc = [&](ParseFuncParams params) {
		Animation newAnimation;
		newAnimation.setName(FileCommandLoader::readString(params));
		animations.push_back(newAnimation);
	};
	fileLoader.addCommand(animationCommand);

	Command spriteSheetCommand;
	spriteSheetCommand.command = "sprite_sheet";
	spriteSheetCommand.parseFunc = [&](ParseFuncParams params) {
		animations.back().setSpriteSheetName(FileCommandLoader::readString(params));
	};
	fileLoader.addCommand(spriteSheetCommand);

	Command sizeCommand;
	sizeCommand.command = "size";
	sizeCommand.parseFunc = [&](ParseFuncParams params) {
		animations.back().setSize(FileCommandLoader::readSize(params));
	};
	fileLoader.addCommand(sizeCommand);

	Command fpsCommand;
	fpsCommand.command = "fps";
	fpsCommand.parseFunc = [&](ParseFuncParams params) {
		animations.back().setFPS(FileCommandLoader::readInt(params));
	};
	fileLoader.addCommand(fpsCommand);

	Command framesCommand;
	framesCommand.command = "frame_keys";
	framesCommand.commandType = COMMAND_MULTI_LINE;
	framesCommand.parseFunc = [&](ParseFuncParams params) {
		animations.back().addFrame(params.line);
	};
	fileLoader.addCommand(framesCommand);

	Command overlayCommand;
	overlayCommand.command = "overlay";
	overlayCommand.parseFunc = [&](ParseFuncParams params) {
		animations.back().setOverlay(FileCommandLoader::readString(params));
	};
	fileLoader.addCommand(overlayCommand);

	Command frameColorsCommand;
	frameColorsCommand.command = "frame_colors";
	frameColorsCommand.commandType = COMMAND_MULTI_LINE;
	frameColorsCommand.parseFunc = [&](ParseFuncParams params) {
		animations.back().addFrameColor(FileCommandLoader::readColor(params));
	};
	fileLoader.addCommand(frameColorsCommand);

	Command overlayColorCommand;
	overlayColorCommand.command = "overlay_colors";
	overlayColorCommand.commandType = COMMAND_MULTI_LINE;
	overlayColorCommand.parseFunc = [&](ParseFuncParams params) {
		animations.back().addOverlayFrameColor(FileCommandLoader::readColor(params));
	};
	fileLoader.addCommand(overlayColorCommand);

	Command frameOffsetsCommand;
	frameOffsetsCommand.command = "frame_offsets";
	frameOffsetsCommand.commandType = COMMAND_MULTI_LINE;
	frameOffsetsCommand.parseFunc = [&](ParseFuncParams params) {
		animations.back().addFrameOffset(FileCommandLoader::readSize(params));
	};
	fileLoader.addCommand(frameOffsetsCommand);

	assert(fileLoader.loadFile(animationFilePath));
	return animations;
}
