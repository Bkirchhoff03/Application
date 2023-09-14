/*
 * Screen.h
 *
 *  Created on: Jun 29, 2023
 *      Author: nebk2
 */

#ifndef GRAPHICS_SCREEN_H_
#define GRAPHICS_SCREEN_H_

#include <stdint.h>
#include "ScreenBuffer.h"
#include "Color.h"
#include <functional>
#include <vector>
#include <string>

class Vec2D;
class Line2D;
class Triangle;
class AARectangle;
class Circle;
class Star;
struct SDL_Window;
struct SDL_Surface;
class BMPImage;
class SpriteSheet;
struct Sprite;
class BitmapFont;

struct SDL_Renderer;
struct SDL_PixelFormat;
struct SDL_Texture;

class Screen {
public:
	Screen();
	~Screen();

	SDL_Window* init(uint32_t w, uint32_t h, uint32_t mag, bool fast = true);
	void swapScreens();

	inline void setClearColor(const Color &clearColor) {
		mClearColor = clearColor;
	}
	inline const uint32_t width() const {
		return mWidth;
	}
	inline const uint32_t height() const {
		return mHeight;
	}

	//Draw Methods Go Here

	void draw(int x, int y, const Color &color);
	void draw(const Vec2D &point, const Color &color);
	void draw(const Line2D &line, const Color &color);
	void draw(const Triangle &triangle, const Color &color, bool fill = false, const Color &fillColor = Color::white());
	void draw(const AARectangle &rect, const Color &color, bool fill = false, const Color &fillColor = Color::white());
	void draw(const Circle &circle, const Color &color, bool fill = false, const Color &fillColor = Color::white());
	void draw(const Star &star, const Color &color, bool fill = false, const Color &fillColor = Color::white());

	void draw(const BMPImage &image, const Sprite &sprite, const Vec2D &pos, const Color &overlayColor =
			Color::white());
	void draw(const SpriteSheet &ss, const std::string &spriteName, const Vec2D &pos, const Color &overlayColor =
			Color::white());
	void draw(const BitmapFont &font, const std::string &textLine, const Vec2D &atPosition, const Color &overlayColor =
			Color::white());
private:

	Screen(const Screen &screen);
	Screen& operator=(const Screen &screen);

	void clearScreen();

	using fillPolyFunc = std::function<Color (uint32_t x, uint32_t y)>;

	void fillPoly(const std::vector<Vec2D> &points, fillPolyFunc func);

	uint32_t mWidth;
	uint32_t mHeight;

	Color mClearColor;
	ScreenBuffer mBackBuffer;

	SDL_Window *moptrWindow;
	SDL_Surface *mnoptrWindowSurface;

	SDL_Renderer *mRenderer;
	SDL_PixelFormat *mPixelFormat;
	SDL_Texture *mTexture;
	bool mFast;
};

#endif /* GRAPHICS_SCREEN_H_ */
