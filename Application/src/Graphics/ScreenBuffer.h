/*
 * ScreenBuffer.h
 *
 *  Created on: Jun 29, 2023
 *      Author: nebk2
 */

#ifndef GRAPHICS_SCREENBUFFER_H_
#define GRAPHICS_SCREENBUFFER_H_

#include <stdint.h>

#include "Color.h"
struct SDL_Surface;

class ScreenBuffer{
public:
	ScreenBuffer();
	ScreenBuffer(const ScreenBuffer& screenBuffer);
	~ScreenBuffer();

	ScreenBuffer& operator=(const ScreenBuffer& sceenBuffer);

	void init(uint32_t format, uint32_t width, uint32_t height);

	inline SDL_Surface * getSurface() {return mSurface;}

	void clear(const Color& c = Color::black());
	//void clear(const Color& c);

	ScreenBuffer(ScreenBuffer && otherBuffer);
	ScreenBuffer& operator=(ScreenBuffer && otherBuffer);

	void setPixel(const Color& color, int x, int y);
private:
	SDL_Surface * mSurface;
	uint32_t getIndex(int r, int c);
};



#endif /* GRAPHICS_SCREENBUFFER_H_ */
