/*
 * Tetris.h
 *
 *  Created on: Jul 18, 2023
 *      Author: nebk2
 */

#ifndef GAMES_TETRIS_TETRIS_H_
#define GAMES_TETRIS_TETRIS_H_

#include "Tetromino.h"
#include "Game.h"
#include "Vec2D.h"
#include "App.h"
#include "GameField.h"
#include <stdint.h>
#include <vector>
#include <iostream>

enum TetrisGameState {
	IN_BEGINNING = 0, IN_FALLING, IN_CLEARING, IN_GAME_END
};

class Tetris: public Game {
public:
	Tetris();
	virtual void init(GameController &controller) override;
	virtual void update(uint32_t dt) override;
	virtual void draw(Screen &screen) override;
	virtual const std::string& getName() const override;
private:
	void resetGame();
	void setToClearingState();
	std::shared_ptr<Tetromino> getRandTetromino();
	bool isGameOver() const;

	GameField mGameField;
	const float INITIAL_FALL_SPEED = 1;
	const Vec2D INITIAL_FALL_VELOCITY = Vec2D(0, App::singleton().height()/20.0f);
	TetrisGameState mGameState;
	std::shared_ptr<Tetromino> mCurrentFallingBlock;

	//TetrominoType mTetrominoType;
};

#endif /* GAMES_TETRIS_TETRIS_H_ */
