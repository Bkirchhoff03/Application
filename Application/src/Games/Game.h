/*
 * Game.h
 *
 *  Created on: Jul 13, 2023
 *      Author: nebk2
 */

#ifndef GAMES_GAME_H_
#define GAMES_GAME_H_

#include <string>
#include <stdint.h>

class GameController;
class Screen;

class Game{
public:
	virtual ~Game(){}
	virtual void init(GameController& controller) = 0;
	virtual void update(uint32_t dt) = 0;
	virtual void draw(Screen& screen) = 0;
	virtual const std::string& getName() const = 0;
};



#endif /* GAMES_GAME_H_ */
