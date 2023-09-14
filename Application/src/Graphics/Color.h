/*
 * Color.h
 *
 *  Created on: Jun 29, 2023
 *      Author: nebk2
 */

#ifndef GRAPHICS_COLOR_H_
#define GRAPHICS_COLOR_H_

#include <stdint.h>

struct SDL_PixelFormat;

class Color{
	uint32_t mColor;
public:

	static const SDL_PixelFormat* mFormat;
	static void initColorFormat(const SDL_PixelFormat * format);

	static Color evaluate1MinusSourceAlpha(const Color& source, const Color& destination);

	static Color black() {return Color(0,0,0,255);}
	static Color white() {return Color(255,255,255,255);}
	static Color blue() {return Color(0,0,255,255);}
	static Color green() {return Color(0,255,0,255);}
	static Color red() {return Color(255,0,0,255);}
	static Color yellow() {return Color(255,255,0,255);}
	static Color magenta() {return Color(255,0,255,255);}
	static Color cyan() {return Color(37,240,217,255);}
	static Color pink() {return Color(252,197,224,255);}
	static Color orange() {return Color(245,190,100,255);}
	static Color purple() {return Color(191, 64, 191, 255);}

	Color(): Color(0){}
	//Color(const Color& color):mColor(color.mColor){}
	Color(uint32_t color): mColor(color){}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	inline void copyFrom(Color& color) {mColor = color.mColor;}
	inline bool operator==(const Color& c) const {return mColor == c.mColor;}
	inline bool operator!=(const Color& c) const {return !(*this == c);}
	inline uint32_t getPixelColor() const {return mColor;}

	void setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	void setRed(uint8_t red);
	void setGreen(uint8_t green);
	void setBlue(uint8_t blue);
	void setAlpha(uint8_t alpha);

	uint8_t getRed() const;
	uint8_t getGreen() const;
	uint8_t getBlue() const;
	uint8_t getAlpha() const;
};



#endif /* GRAPHICS_COLOR_H_ */
