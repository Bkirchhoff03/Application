/*
 * ButtonOptionsScene.cpp
 *
 *  Created on: Aug 10, 2023
 *      Author: nebk2
 */

#include "ButtonOptionsScene.h"
#include "App.h"
#include "BitmapFont.h"
#include "Utils.h"

ButtonOptionsScene::ButtonOptionsScene(const std::vector<std::string> &optionNames, const Color &textColor) :
		mHighlightedOption(0) {
	const BitmapFont &font = App::singleton().getFont();
	for (size_t i = 0; i < optionNames.size(); ++i) {
		mButtons.push_back(Button(font, textColor));
		mButtons.back().setButtonText(optionNames[i]);
	}
	if (optionNames.size() > 0) {
		mButtons[mHighlightedOption].setHighlighted(true);
	}
}

void ButtonOptionsScene::init() {
	ButtonAction upAction;
	upAction.key = GameController::upKey();
	upAction.action = [this](uint32_t dt, InputState state) {
		if (GameController::isPressed(state)) {
			setPreviousButtonHighlighted();
		}
	};
	mGameController.addInputActionForKey(upAction);

	ButtonAction downAction;
	downAction.key = GameController::downKey();
	downAction.action = [this](uint32_t dt, InputState state) {
		if (GameController::isPressed(state)) {
			setNextButtonHighlighted();
		}
	};
	mGameController.addInputActionForKey(downAction);

	ButtonAction acceptAction;
	acceptAction.key = GameController::actionKey();
	acceptAction.action = [this](uint32_t dt, InputState state) {
		if (GameController::isPressed(state)) {
			executeCurrentButtonAction();
		}
	};
	mGameController.addInputActionForKey(acceptAction);

	uint32_t height = App::singleton().height();
	uint32_t width = App::singleton().width();

	const BitmapFont &font = App::singleton().getFont();

	Size fontSize = font.getSizeOf(mButtons[0].getButtonText());

	const int BUTTON_PAD = 10;

	unsigned int buttonHeight = fontSize.height + BUTTON_PAD * 2;
	uint32_t maxButtonWidth = fontSize.width;

	for (const Button &button : mButtons) {
		Size s = font.getSizeOf(button.getButtonText());

		if (s.width > maxButtonWidth) {
			maxButtonWidth = s.width;
		}
	}

	maxButtonWidth += BUTTON_PAD * 2;
	const uint32_t Y_PAD = 1;
	uint32_t yOffset = height / 2 - ((buttonHeight + Y_PAD) * static_cast<uint32_t>(mButtons.size())) / 2;

	for (Button &button : mButtons) {
		button.init(Vec2D(width / 2 - maxButtonWidth / 2, yOffset), maxButtonWidth, buttonHeight);

		yOffset += buttonHeight + Y_PAD;
	}
	mButtons[mHighlightedOption].setHighlighted(true);

}
void ButtonOptionsScene::update(uint32_t dt) {

}
void ButtonOptionsScene::draw(Screen &theScreen) {
	for (Button &button : mButtons) {
		button.draw(theScreen);
	}
}

void ButtonOptionsScene::setButtonActions(const std::vector<Button::ButtonAction> &buttonActions) {
	for (size_t i = 0; i < mButtons.size(); ++i) {
		mButtons[i].setButtonAction(buttonActions[i]);
	}
}

void ButtonOptionsScene::setNextButtonHighlighted() {
	mHighlightedOption = (mHighlightedOption + 1) % mButtons.size();
	highlightCurrentButton();
}
void ButtonOptionsScene::setPreviousButtonHighlighted() {
	--mHighlightedOption;
	if (mHighlightedOption < 0) {
		mHighlightedOption = static_cast<int>(mButtons.size()) - 1;
	}

	highlightCurrentButton();
}
void ButtonOptionsScene::executeCurrentButtonAction() {
	mButtons[mHighlightedOption].executeAction();
}
void ButtonOptionsScene::highlightCurrentButton() {
	for (Button &button : mButtons) {
		button.setHighlighted(false);
	}
	mButtons[mHighlightedOption].setHighlighted(true);
}
