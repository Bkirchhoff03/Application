/*
 * Color.cpp
 *
 *  Created on: Jun 29, 2023
 *      Author: nebk2
 */

#include "Color.h"
#include <SDL2/SDL.h>

const SDL_PixelFormat *Color::mFormat = nullptr;
void Color::initColorFormat(const SDL_PixelFormat *format) {
	Color::mFormat = format;
}
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	setRGBA(r, g, b, a);
}
void Color::setRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	mColor = SDL_MapRGBA(mFormat, r, g, b, a);
}

void Color::setRed(uint8_t red) {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
	setRGBA(red, g, b, a);
}
void Color::setGreen(uint8_t green) {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
	setRGBA(r, green, b, a);
}
void Color::setBlue(uint8_t blue) {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
	setRGBA(r, g, blue, a);
}
void Color::setAlpha(uint8_t alpha) {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
	setRGBA(r, g, b, alpha);
}

uint8_t Color::getRed() const {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
	return r;
}
uint8_t Color::getGreen() const {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
	return g;
}
uint8_t Color::getBlue() const {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
	return b;
}
uint8_t Color::getAlpha() const {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	SDL_GetRGBA(mColor, mFormat, &r, &g, &b, &a);
	return a;
}

Color Color::evaluate1MinusSourceAlpha(const Color& source, const Color& destination){
	uint8_t alpha = source.getAlpha();

	float sourceAlpha = float(alpha) / (255.0f);
	float destAlpha = (1.0f) - sourceAlpha;

	Color outColor;

	outColor.setAlpha(255);
	outColor.setRed((float(source.getRed()) * sourceAlpha) + (destination.getRed() * destAlpha));
	outColor.setGreen((float(source.getGreen())) * sourceAlpha + (destination.getGreen() * destAlpha));
	outColor.setBlue((float(source.getBlue())) * sourceAlpha + (destination.getBlue() * destAlpha));

	return outColor;
}
