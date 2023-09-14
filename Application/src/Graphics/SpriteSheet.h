/*
 * SpriteSheet.h
 *
 *  Created on: Aug 7, 2023
 *      Author: nebk2
 */

#ifndef GRAPHICS_SPRITESHEET_H_
#define GRAPHICS_SPRITESHEET_H_

#include "BMPImage.h"
#include <string>
#include <vector>
#include <stdint.h>

struct Sprite {
	uint32_t xPos = 0;
	uint32_t yPos = 0;
	uint32_t width = 0;
	uint32_t height = 0;
};

class SpriteSheet {
public:
	SpriteSheet();
	bool load(const std::string &name);
	Sprite getSprite(const std::string &spriteName) const;

	std::vector<std::string> spriteNames() const;

	inline const BMPImage& getBMPImage() const {
		return mBMPImage;
	}
	inline uint32_t getWidth() const {
		return mBMPImage.getWidth();
	}
	inline uint32_t getHeight() const {
		return mBMPImage.getWidth();
	}
	
private:
	bool loadSpriteSections(const std::string &path);
	struct BMPImageSection {
		std::string key = "";
		Sprite sprite;
	};

	BMPImage mBMPImage;
	std::vector<BMPImageSection> mSections;
};

#endif /* GRAPHICS_SPRITESHEET_H_ */
