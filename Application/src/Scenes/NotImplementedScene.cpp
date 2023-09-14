/*
 * NotImplementedScene.cpp
 *
 *  Created on: Aug 10, 2023
 *      Author: nebk2
 */

#include "NotImplementedScene.h"
#include "App.h"
#include "AARectangle.h"
#include "InputAction.h"
#include "Screen.h"

void NotImplementedScene::init() {
	ButtonAction backAction;
	backAction.key = GameController::cancelKey();
	backAction.action = [this](uint32_t dt, InputState state) {
		if (GameController::isPressed(state)) {
			App::singleton().popScene();
		}
	};

	mGameController.addInputActionForKey(backAction);
}
void NotImplementedScene::update(uint32_t dt) {

}
void NotImplementedScene::draw(Screen &theScreen) {
	const BitmapFont &font = App::singleton().getFont();
	AARectangle rect = { Vec2D::zero, App::singleton().width(), App::singleton().height() };

	Vec2D textDrawPosition;
	textDrawPosition = font.getDrawPosition(getSceneName(), rect, BFXA_CENTER, BFYA_CENTER);
	theScreen.draw(font, getSceneName(), textDrawPosition, Color::red());
}
const std::string& NotImplementedScene::getSceneName() const {
	static std::string name = "Not Implemented!";
	return name;
}

