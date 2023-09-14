/*
 * GameController.cpp
 *
 *  Created on: Jul 11, 2023
 *      Author: nebk2
 */

#include "GameController.h"
#include <SDL2/SDL.h>

GameController::GameController():mMouseMovedAction(nullptr) {

}
InputAction GameController::getActionForKey(InputKey key) {
	for (const ButtonAction &buttonAction : mButtonActions) {
		if (key == buttonAction.key) {
			return buttonAction.action;
		}
	}
	return [](uint32_t, InputState) {
	};
}
void GameController::addInputActionForKey(const ButtonAction &buttonAction) {
	mButtonActions.push_back(buttonAction);
}
void GameController::clearAll() {
	mButtonActions.clear();
}

bool GameController::isPressed(InputState state) {
	return state == SDL_PRESSED;
}
bool GameController::isReleased(InputState state) {
	return state == SDL_RELEASED;
}
InputKey GameController::actionKey() {
	return static_cast<InputKey>(SDLK_a);
}
InputKey GameController::cancelKey() {
	return static_cast<InputKey>(SDLK_s);
}
InputKey GameController::leftKey() {
	return static_cast<InputKey>(SDLK_LEFT);
}
InputKey GameController::rightKey() {
	return static_cast<InputKey>(SDLK_RIGHT);
}
InputKey GameController::upKey() {
	return static_cast<InputKey>(SDLK_UP);
}
InputKey GameController::downKey() {
	return static_cast<InputKey>(SDLK_DOWN);
}

MouseInputAction GameController::getMouseButtonActionForMouseButton(MouseButton button) {
	for (MouseButtonAction buttonAction : mMouseButtonActions) {
		if(button == buttonAction.mouseButton){
			return buttonAction.mouseInputAction;
		}
	}
	return [](InputState state, const MousePosition& pos){};
}
void GameController::addMouseButtonAction(
		const MouseButtonAction &mouseButtonAction) {
	mMouseButtonActions.push_back(mouseButtonAction);
}

MouseButton GameController::leftMouseButton() {
	return static_cast<MouseButton>(SDL_BUTTON_LEFT);
}
MouseButton GameController::rightMouseButton() {
	return static_cast<MouseButton>(SDL_BUTTON_RIGHT);
}
