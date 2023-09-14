/*
 * InputController.cpp
 *
 *  Created on: Jul 12, 2023
 *      Author: nebk2
 */

#include "InputController.h"
#include <SDL2/SDL.h>
#include "GameController.h"
#include <iostream>

InputController::InputController() :
		mQuit(nullptr), mnoptrCurrentController(nullptr) {

}
void InputController::init(InputAction quitAction) {
	mQuit = quitAction;
}
void InputController::update(uint32_t dt) {
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_QUIT: {
			mQuit(dt, SDL_PRESSED);
		}
			break;
		case SDL_MOUSEMOTION: {
			if (mnoptrCurrentController) {
				if (MouseMovedAction mouseMoved = mnoptrCurrentController->getMouseMovedAction()) {
					MousePosition position;
					position.xPos = sdlEvent.motion.x;
					position.yPos = sdlEvent.motion.y;
					mouseMoved(position);
				}
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			if (mnoptrCurrentController) {
				MouseInputAction action = mnoptrCurrentController->getMouseButtonActionForMouseButton(
						static_cast<MouseButton>(sdlEvent.button.button));

				MousePosition position;
				position.xPos = sdlEvent.button.x;
				position.yPos = sdlEvent.button.y;

				action(static_cast<InputState>(sdlEvent.button.state), position);
			}
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:

			if (mnoptrCurrentController) {
				InputAction action = mnoptrCurrentController->getActionForKey(sdlEvent.key.keysym.sym);

				action(dt, static_cast<InputState>(sdlEvent.key.state));
			}
			break;
		default:
			break;
		}
	}
}
void InputController::setGameController(GameController *controller) {
	mnoptrCurrentController = controller;
}

