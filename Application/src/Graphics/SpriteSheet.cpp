/*
 * SpriteSheet.cpp
 *
 *  Created on: Aug 7, 2023
 *      Author: nebk2
 */

#include "SpriteSheet.h"
#include "FileCommandLoader.h"
#include "Utils.h"
#include "App.h"

SpriteSheet::SpriteSheet() {
}
bool SpriteSheet::load(const std::string &name) {
	bool loadedImage = mBMPImage.load(App::singleton().getBasePath() + std::string("Assets\\") + name + ".bmp");
	bool loadedSpriteSections = loadSpriteSections(
			App::singleton().getBasePath() + std::string("Assets/") + name + ".txt");
	return loadedImage && loadedSpriteSections;

}
Sprite SpriteSheet::getSprite(const std::string &spriteName) const {
	size_t length = mSections.size();

	for (size_t i = 0; i < length; ++i) {
		if (stringCompare(mSections[i].key, spriteName)) {
			return mSections[i].sprite;
		}
	}

	return Sprite();
}

std::vector<std::string> SpriteSheet::spriteNames() const {
	std::vector<std::string> spriteName;

	for (const BMPImageSection &section : mSections) {
		spriteName.push_back(section.key);
	}
	return spriteName;
}
bool SpriteSheet::loadSpriteSections(const std::string &path) {
	FileCommandLoader fileLoader;

	Command spriteCommand;
	spriteCommand.command = "sprite";
	spriteCommand.parseFunc = [&](ParseFuncParams params) {
		BMPImageSection section;
		mSections.push_back(section);
	};
	fileLoader.addCommand(spriteCommand);

	Command keyCommand;
	keyCommand.command = "key";
	keyCommand.parseFunc = [&](ParseFuncParams params) {
		mSections.back().key = FileCommandLoader::readString(params);
	};
	fileLoader.addCommand(keyCommand);

	Command xPosCommand;
	xPosCommand.command = "xPos";
	xPosCommand.parseFunc = [&](ParseFuncParams params) {
		mSections.back().sprite.xPos = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(xPosCommand);

	Command yPosCommand;
	yPosCommand.command = "yPos";
	yPosCommand.parseFunc = [&](ParseFuncParams params) {
		mSections.back().sprite.yPos = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(yPosCommand);

	Command widthCommand;
	widthCommand.command = "width";
	widthCommand.parseFunc = [&](ParseFuncParams params) {
		mSections.back().sprite.width = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(widthCommand);

	Command heightCommand;
	heightCommand.command = "height";
	heightCommand.parseFunc = [&](ParseFuncParams params) {
		mSections.back().sprite.height = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(heightCommand);

	return fileLoader.loadFile(path);
}
