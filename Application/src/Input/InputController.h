/*
 * InputController.h
 *
 *  Created on: Jul 12, 2023
 *      Author: nebk2
 */

#ifndef INPUT_INPUTCONTROLLER_H_
#define INPUT_INPUTCONTROLLER_H_

#include "InputAction.h"

class GameController;

class InputController{
public:

	InputController();
	void init(InputAction quitAction);
	void update(uint32_t dt);
	void setGameController(GameController* controller);

private:
	InputAction mQuit;
	GameController* mnoptrCurrentController;
};



#endif /* INPUT_INPUTCONTROLLER_H_ */
