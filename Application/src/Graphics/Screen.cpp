/*
 * Screen.cpp
 *
 *  Created on: Jun 29, 2023
 *      Author: nebk2
 */
#include "Screen.h"
#include "Vec2D.h"
#include "Line2D.h"
#include "Triangle.h"
#include "AARectangle.h"
#include "BMPImage.h"
#include "Circle.h"
#include "Star.h"
#include "Utils.h"
#include "Color.h"
#include "SpriteSheet.h"
#include "BitmapFont.h"
#include <SDL2/SDL.h>
#include <cassert>
#include <cmath>
#include <algorithm>
Screen::Screen() :
		mWidth(0), mHeight(0), moptrWindow(nullptr), mnoptrWindowSurface(nullptr), mRenderer(nullptr), mPixelFormat(
				nullptr), mTexture(nullptr), mFast(true) {

}
Screen::~Screen() {
	if (mPixelFormat) {
		SDL_FreeFormat(mPixelFormat);
		mPixelFormat = nullptr;
	}
	if (mTexture) {
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}
	if (mRenderer) {
		SDL_DestroyRenderer(mRenderer);
		mRenderer = nullptr;
	}
	if (moptrWindow) {
		SDL_DestroyWindow(moptrWindow);
		moptrWindow = nullptr;
	}
	SDL_Quit();
}

SDL_Window* Screen::init(uint32_t w, uint32_t h, uint32_t mag, bool fast) {
	mFast = fast;

	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cout << "Error SDL_Init Failed" << std::endl;
		return nullptr;
	}
	mWidth = w;
	mHeight = h;
	moptrWindow = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, mWidth * mag, mHeight * mag, 0);

	if (moptrWindow) {
		uint8_t rClear = 0;
		uint8_t gClear = 0;
		uint8_t bClear = 0;
		uint8_t aClear = 255;
		if (mFast) {
			mRenderer = SDL_CreateRenderer(moptrWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (mRenderer == nullptr) {
				std::cout << "SDL_CreateRenderer failed" << std::endl;
				return nullptr;
			}
			SDL_SetRenderDrawColor(mRenderer, rClear, gClear, bClear, aClear);
		} else {
			mnoptrWindowSurface = SDL_GetWindowSurface(moptrWindow);
		}

		//mPixelFormat = SDL_AllocFormat(SDL_GetWindowPixelFormat(moptrWindow));
		mPixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);
		if (mFast) {
			mTexture = SDL_CreateTexture(mRenderer, mPixelFormat->format, SDL_TEXTUREACCESS_STREAMING, w, h);
		}
		Color::initColorFormat(mPixelFormat);

		mClearColor = Color(rClear, gClear, bClear, aClear);

		mBackBuffer.init(mPixelFormat->format, mWidth, mHeight);

		mBackBuffer.clear(mClearColor);
	}

	return moptrWindow;
}
void Screen::swapScreens() {
	assert(moptrWindow);
	if (moptrWindow) {
		clearScreen();
		if (mFast) {
			uint8_t *textureData = nullptr;
			int texturePitch = 0;
			if (SDL_LockTexture(mTexture, nullptr, (void**) &textureData, &texturePitch) >= 0) {
				SDL_Surface *surface = mBackBuffer.getSurface();
				memcpy(textureData, surface->pixels, surface->w * surface->h * mPixelFormat->BytesPerPixel);
				SDL_UnlockTexture(mTexture);
				SDL_RenderCopy(mRenderer, mTexture, nullptr, nullptr);
				SDL_RenderPresent(mRenderer);
			}
		} else {
			SDL_BlitScaled(mBackBuffer.getSurface(), nullptr, mnoptrWindowSurface, nullptr);

			SDL_UpdateWindowSurface(moptrWindow);
		}
		mBackBuffer.clear(mClearColor);
	}
}
void Screen::draw(int x, int y, const Color &color) {
	assert(moptrWindow);
	if (moptrWindow) {
		mBackBuffer.setPixel(color, x, y);
	}
}
void Screen::draw(const Vec2D &point, const Color &color) {
	assert(moptrWindow);
	if (moptrWindow) {
		mBackBuffer.setPixel(color, point.GetX(), point.GetY());
	}
}

void Screen::draw(const Line2D &line, const Color &color) {
	assert(moptrWindow);
	if (moptrWindow) {
		int dx, dy;

		int x0 = roundf(line.getP0().GetX());
		int y0 = roundf(line.getP0().GetY());
		int x1 = roundf(line.getP1().GetX());
		int y1 = roundf(line.getP1().GetY());

		dx = x1 - x0;
		dy = y1 - y0;
		// evaluate to 1 or -1, increment axis, going each way depending on slope direction
		signed const char ix((dx > 0) - (dx < 0));
		signed const char iy((dy > 0) - (dy < 0));

		dx = abs(dx) * 2;
		dy = abs(dy) * 2;
		//first point in line
		draw(x0, y0, color);
		if (dx >= dy) {
			//go along in the x

			int d = dy - dx / 2;
			while (x0 != x1) {
				if (d >= 0) {
					d -= dx;
					y0 += iy;
				}
				d += dy;
				x0 += ix;
				draw(x0, y0, color);
			}
		} else {
			//go along in the y
			int d = dx - dy / 2;
			while (y0 != y1) {
				if (d >= 0) {
					d -= dy;
					x0 += ix;
				}
				d += dx;
				y0 += iy;
				draw(x0, y0, color);
			}
		}
	}
}

void Screen::draw(const Triangle &triangle, const Color &color, bool fill, const Color &fillColor) {
	if (fill) {
		fillPoly(triangle.getPoints(), [fillColor](uint32_t x, uint32_t y) {
			return fillColor;
		});
	}
	draw(Line2D(triangle.getP0(), triangle.getP1()), color);
	draw(Line2D(triangle.getP1(), triangle.getP2()), color);
	draw(Line2D(triangle.getP2(), triangle.getP0()), color);

}

void Screen::draw(const AARectangle &rect, const Color &color, bool fill, const Color &fillColor) {
	if (fill) {
		fillPoly(rect.getPoints(), [fillColor](uint32_t x, uint32_t y) {
			return fillColor;
		});
	}
	std::vector<Vec2D> vector = rect.getPoints();
	draw(Line2D(vector[0], vector[1]), color);
	draw(Line2D(vector[1], vector[2]), color);
	draw(Line2D(vector[2], vector[3]), color);
	draw(Line2D(vector[3], vector[0]), color);
}

void Screen::draw(const Circle &circle, const Color &color, bool fill, const Color &fillColor) {

	static unsigned int NUM_CIRCLE_SEGMENTS = 30;

	std::vector<Vec2D> circlePoints;
	std::vector<Line2D> lines;

	float angle = TWO_PI / float(NUM_CIRCLE_SEGMENTS);

	Vec2D p0 = Vec2D(circle.getCenterPoint().GetX() + circle.getRadius(), circle.getCenterPoint().GetY());
	Vec2D p1 = p0;
	Line2D nextLineToDraw;

	for (unsigned int i = 0; i < NUM_CIRCLE_SEGMENTS; i++) {
		p1.rotate(angle, circle.getCenterPoint());
		nextLineToDraw.setP1(p1);
		nextLineToDraw.setP0(p0);

		lines.push_back(nextLineToDraw);
		p0 = p1;
		circlePoints.push_back(p0);
	}
	if (fill) {
		fillPoly(circlePoints, [fillColor](uint32_t x, uint32_t y) {
			return fillColor;
		});
	}

	for (const Line2D &line : lines) {
		draw(line, color);
	}
}

void Screen::draw(const Star &star, const Color &color, bool fill, const Color &fillColor) {
	Star nStar = star;
	std::vector<Vec2D> points = nStar.getPoints();
	draw(Line2D(points[0], points[5]), color);
	draw(Line2D(points[5], points[1]), color);
	draw(Line2D(points[1], points[6]), color);
	draw(Line2D(points[6], points[2]), color);
	draw(Line2D(points[2], points[7]), color);
	draw(Line2D(points[7], points[3]), color);
	draw(Line2D(points[3], points[8]), color);
	draw(Line2D(points[8], points[4]), color);
	draw(Line2D(points[4], points[9]), color);
	draw(Line2D(points[9], points[0]), color);
	if (fill) {
		fillPoly(points, [fillColor](uint32_t x, uint32_t y) {
			return fillColor;
		});
	}
}

void Screen::draw(const BMPImage &image, const Sprite &sprite, const Vec2D &pos, const Color &overlayColor) {
	float rVal = static_cast<float>(overlayColor.getRed()) / 255.0f;
	float gVal = static_cast<float>(overlayColor.getGreen()) / 255.0f;
	float bVal = static_cast<float>(overlayColor.getBlue()) / 255.0f;
	float aVal = static_cast<float>(overlayColor.getAlpha()) / 255.0f;
	uint32_t width = sprite.width;
	uint32_t height = sprite.height;

	const std::vector<Color> &pixels = image.getPixels();

	auto topLeft = pos;
	auto topRight = pos + Vec2D(width, 0);
	auto bottomLeft = pos + Vec2D(0, height);
	auto bottomRight = pos + Vec2D(width, height);

	std::vector<Vec2D> points = { topLeft, bottomLeft, bottomRight, topRight };

	Vec2D xAxis = topRight - topLeft;
	Vec2D yAxis = bottomLeft - topLeft;

	const float invXAxisLengthSq = 1.0f / xAxis.mag2();
	const float invYAxisLengthSq = 1.0f / yAxis.mag2();

	fillPoly(points, [&](uint32_t px, uint32_t py) {

		Vec2D p = { static_cast<float>(px), static_cast<float>(py) };
		Vec2D d = p - topLeft;

		float u = invXAxisLengthSq * d.dot(xAxis);
		float v = invYAxisLengthSq * d.dot(yAxis);

		u = clamp(u, 0.0f, 1.0f);
		v = clamp(v, 0.0f, 1.0f);

		float tx = roundf(u * static_cast<float>(sprite.width));
		float ty = roundf(v * static_cast<float>(sprite.height));
		Color imageColor = pixels[getIndex(image.getWidth(), ty + sprite.yPos, tx + sprite.xPos)];
		Color newColor = { static_cast<uint8_t>(imageColor.getRed() * rVal), static_cast<uint8_t>(imageColor.getGreen() * gVal),
	static_cast<uint8_t>(imageColor.getBlue() * bVal),
	static_cast<uint8_t>(imageColor.getAlpha() * aVal)};
		return newColor;
	});
}
void Screen::draw(const SpriteSheet &ss, const std::string &spriteName, const Vec2D &pos, const Color &overlayColor) {
	draw(ss.getBMPImage(), ss.getSprite(spriteName), pos, overlayColor);
}
void Screen::clearScreen() {
	assert(moptrWindow);
	if (moptrWindow) {
		if (mFast) {
			SDL_RenderClear(mRenderer);
		} else {
			SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.getPixelColor());
		}
	}
}

void Screen::draw(const BitmapFont &font, const std::string &textLine, const Vec2D &atPosition,
		const Color &overlayColor) {
	uint32_t xPos = atPosition.GetX();
	const SpriteSheet &ss = font.getSpriteSheet();

	for (char c : textLine) {
		if (c == ' ') {
			xPos += font.getFontSpacingBetweenWords();
			continue;
		}
		Sprite sprite = ss.getSprite(std::string("") + c);
		draw(ss.getBMPImage(), sprite, Vec2D(xPos, atPosition.GetY()), overlayColor);
		xPos += sprite.width;

		xPos += font.getFontSpacingBetweenLetters();
	}
}

void Screen::fillPoly(const std::vector<Vec2D> &points, fillPolyFunc func) {
	if (points.size() > 0) {
		float top = points[0].GetY();
		float bottom = points[0].GetY();
		float right = points[0].GetX();
		float left = points[0].GetX();

		for (size_t i = 1; i < points.size(); ++i) {
			if (points[i].GetY() < top) {
				top = points[i].GetY();
			}
			if (points[i].GetY() > bottom) {
				bottom = points[i].GetY();
			}
			if (points[i].GetX() < left) {
				left = points[i].GetX();
			}
			if (points[i].GetX() > right) {
				right = points[i].GetX();
			}
		}

		for (int pixelY = top; pixelY < bottom; ++pixelY) {
			std::vector<float> nodeXVec;

			size_t j = points.size() - 1;

			for (size_t i = 0; i < points.size(); ++i) {
				float pointiY = points[i].GetY();
				float pointjY = points[j].GetY();

				if ((pointiY <= (float) pixelY && pointjY > (float) pixelY)
						|| (pointjY <= (float) pixelY && pointiY > (float) pixelY)) {
					float denom = pointjY - pointiY;
					if (isEqual(denom, 0)) {
						continue;
					}

					float x = points[i].GetX() + (pixelY - pointiY) / (denom) * (points[j].GetX() - points[i].GetX());
					nodeXVec.push_back(x);
				}
				j = i;
			}
			std::sort(nodeXVec.begin(), nodeXVec.end(), std::less<>());

			for (size_t k = 0; k < nodeXVec.size(); k += 2) {
				if (nodeXVec[k] > right) {
					break;
				}

				if (nodeXVec[k + 1] > left) {
					if (nodeXVec[k] < left) {
						nodeXVec[k] = left;
					}
					if (nodeXVec[k + 1] > right) {
						nodeXVec[k + 1] = right;
					}

					for (int pixelX = nodeXVec[k]; pixelX < nodeXVec[k + 1]; ++pixelX) {
						draw(pixelX, pixelY, func(pixelX, pixelY));
					}
				}
			}
		}

	}
}
