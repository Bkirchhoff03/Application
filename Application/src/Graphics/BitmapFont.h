/*
 * BitmapFont.h
 *
 *  Created on: Aug 8, 2023
 *      Author: nebk2
 */

#ifndef GRAPHICS_BITMAPFONT_H_
#define GRAPHICS_BITMAPFONT_H_

#include "SpriteSheet.h"
#include <stdint.h>
#include "Utils.h"

class Vec2D;
class AARectangle;

enum BitmapFontXAlingment {
	BFXA_LEFT = 0, BFXA_CENTER, BFXA_RIGHT
};

enum BitmapFontYAlingment {
	BFYA_TOP = 0, BFYA_CENTER, BFYA_BOTTOM
};

class BitmapFont {
public:

	BitmapFont() {
	}
	bool load(const std::string &name);
	Size getSizeOf(const std::string &str) const;
	Vec2D getDrawPosition(const std::string &str, const AARectangle &box, BitmapFontXAlingment xAlign = BFXA_LEFT,
			BitmapFontYAlingment yAlign = BFYA_TOP) const;

	inline const SpriteSheet& getSpriteSheet() const {
		return mFontSheet;
	}
	inline const uint32_t getFontSpacingBetweenLetters() const {
		return 2;
	}
	inline const uint32_t getFontSpacingBetweenWords() const {
		return 5;
	}

private:
	SpriteSheet mFontSheet;
};

#endif /* GRAPHICS_BITMAPFONT_H_ */
