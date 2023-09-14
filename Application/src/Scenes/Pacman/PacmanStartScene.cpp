/*
 * PacmanStartScene.cpp
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#include "PacmanStartScene.h"
#include "App.h"
#include "NotImplementedScene.h"
#include "GameScene.h"
#include "PacmanGame.h"
#include <vector>
#include <memory>

PacmanStartScene::PacmanStartScene() :
		ButtonOptionsScene( { "Play Game", "High Scores" }, Color::yellow()) {

}
void PacmanStartScene::init() {
	ButtonAction backAction;
	backAction.key = GameController::cancelKey();
	backAction.action = [this](uint32_t dt, InputState state) {
		if (GameController::isPressed(state)) {
			App::singleton().popScene();
		}
	};

	mGameController.addInputActionForKey(backAction);

	std::vector<Button::ButtonAction> actions;
	actions.push_back([this]() {
		auto pacmanGame = std::make_unique<PacmanGame>();
		App::singleton().pushScene(std::make_unique<GameScene>(std::move(pacmanGame)));
	});

	actions.push_back([this]() {
		App::singleton().pushScene(std::make_unique<NotImplementedScene>());
	});
	setButtonActions(actions);
	ButtonOptionsScene::init();

}
void PacmanStartScene::update(uint32_t dt) {
}
void PacmanStartScene::draw(Screen &theScreen) {
	ButtonOptionsScene::draw(theScreen);
}
const std::string& PacmanStartScene::getSceneName() const {
	static std::string name = "!Pacman start!";
	return name;
}

