/*
 * Button.cpp
 *
 *  Created on: Aug 10, 2023
 *      Author: nebk2
 */

#include "Button.h"
#include "Screen.h"

Button::Button(const BitmapFont &bitmapFont, const Color &textColor, const Color &highlightColor) :
		mBitmapFont(bitmapFont), mTextColor(textColor), mHighlightColor(highlightColor), mTitle(""), mHighlighted(
				false), mAction(nullptr) {
}
void Button::init(Vec2D topLeft, unsigned int width, unsigned int height) {
	mBBox = AARectangle(topLeft, width, height);
}
void Button::draw(Screen &theScreen) {
	theScreen.draw(mBitmapFont, mTitle, mBitmapFont.getDrawPosition(mTitle, mBBox, BFXA_CENTER, BFYA_CENTER),
			mTextColor);
	if (mHighlighted) {
		theScreen.draw(mBBox, mHighlightColor);
	}
}

void Button::executeAction() {
	mAction();
}

