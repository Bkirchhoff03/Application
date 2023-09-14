/*
 * ScreenBuffer.cpp
 *
 *  Created on: Jun 29, 2023
 *      Author: nebk2
 */
#include "ScreenBuffer.h"
#include "Color.h"
#include <SDL2/SDL.h>
#include <cassert>

ScreenBuffer::ScreenBuffer() :
		mSurface(nullptr) {

}

ScreenBuffer::ScreenBuffer(const ScreenBuffer &screenBuffer) {
	mSurface = SDL_CreateRGBSurfaceWithFormat(0, screenBuffer.mSurface->w,
			screenBuffer.mSurface->h, 0, screenBuffer.mSurface->format->format);

	SDL_BlitSurface(screenBuffer.mSurface, nullptr, mSurface, nullptr);
}
ScreenBuffer::~ScreenBuffer() {
	if (mSurface) {
		SDL_FreeSurface(mSurface);
	}
}

ScreenBuffer& ScreenBuffer::operator=(const ScreenBuffer &screenBuffer) {
	if (this == &screenBuffer) {
		return *this;
	}
	if (mSurface != nullptr) {
		SDL_FreeSurface(mSurface);
		mSurface = nullptr;
	}
	if (screenBuffer.mSurface != nullptr) {
		mSurface = SDL_CreateRGBSurfaceWithFormat(0, screenBuffer.mSurface->w,
				screenBuffer.mSurface->h, 0,
				screenBuffer.mSurface->format->format);

		SDL_BlitSurface(screenBuffer.mSurface, nullptr, mSurface, nullptr);
	}

	return *this;
}

void ScreenBuffer::init(uint32_t format, uint32_t width, uint32_t height) {
	mSurface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 0, format);
	clear();
}

void ScreenBuffer::clear(const Color &c) {
	assert(mSurface);
	if (mSurface) {
		SDL_FillRect(mSurface, nullptr, c.getPixelColor());
	}
}
void ScreenBuffer::setPixel(const Color &color, int x, int y) {
	assert(mSurface);
	if (mSurface && (y < mSurface->h && y >= 0 && x >= 0 && x < mSurface->w)) {
		SDL_LockSurface(mSurface);

		uint32_t *pixels = (uint32_t*) mSurface->pixels;

		size_t index = getIndex(y, x);
		Color surfaceColor(pixels[index]); //destinationColor
		Color end = Color::evaluate1MinusSourceAlpha(color, surfaceColor);
		pixels[index] = end.getPixelColor();

		SDL_UnlockSurface(mSurface);
	}
}
uint32_t ScreenBuffer::getIndex(int r, int c) {
	assert(mSurface);
	if (mSurface) {
		return r * mSurface->w + c;
	}
	return 0;
}

ScreenBuffer::ScreenBuffer(ScreenBuffer && otherBuffer): mSurface(otherBuffer.mSurface){
	otherBuffer.mSurface = nullptr;
}

ScreenBuffer& ScreenBuffer::operator=(ScreenBuffer && otherBuffer)
{
    //make sure we're not assigning ourselves
    if (this == &otherBuffer)
    {
        return *this;
    }

    delete mSurface;

    mSurface = otherBuffer.mSurface;

    //make sure to invalidate otherArray
    otherBuffer.mSurface = nullptr;

    return *this;
}
