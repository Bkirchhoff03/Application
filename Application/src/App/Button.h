/*
 * Button.h
 *
 *  Created on: Aug 10, 2023
 *      Author: nebk2
 */

#ifndef APP_BUTTON_H_
#define APP_BUTTON_H_

#include <string>
#include "AARectangle.h"
#include <functional>
#include "Color.h"
#include "BitmapFont.h"

class Screen;

class Button {
public:
	using ButtonAction = std::function<void(void)>;

	Button(const BitmapFont &bitmapFont, const Color &textColor, const Color &highlightColor = Color::white());
	void init(Vec2D topLeft, unsigned int width, unsigned int height);
	void draw(Screen &theScreen);
	void executeAction();

	inline void setButtonText(const std::string &text) {
		mTitle = text;
	}
	inline const std::string& getButtonText() const {
		return mTitle;
	}
	inline void setHighlighted(bool highlighted) {
		mHighlighted = highlighted;
	}
	inline bool isHighlighted() const {
		return mHighlighted;
	}
	inline void setButtonAction(ButtonAction action) {
		mAction = action;
	}

private:
	const BitmapFont &mBitmapFont;
	std::string mTitle;
	AARectangle mBBox;

	bool mHighlighted;
	ButtonAction mAction;
	Color mHighlightColor;
	Color mTextColor;
};



#endif /* APP_BUTTON_H_ */
