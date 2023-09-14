/*
 * Tetris.cpp
 *
 *  Created on: Jul 18, 2023
 *      Author: nebk2
 */

#include "Tetromino.h"
#include "Tetris.h"
#include "GameController.h"
#include "GameField.h"
#include "App.h"
#include "Line2D.h"
#include "Vec2D.h"
#include <random>
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>

Tetris::Tetris() :
		mGameState(IN_BEGINNING) {
}

void Tetris::init(GameController &controller) {
	//mGameField;
	controller.clearAll();
	resetGame();
	ButtonAction startAction;
	startAction.key = GameController::actionKey();
	startAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == IN_BEGINNING) {
			if (GameController::isPressed(state)) {
				mGameState = IN_FALLING;
				mCurrentFallingBlock->setVelocity(INITIAL_FALL_VELOCITY);
			}
		} else if (mGameState == IN_GAME_END) {
			if (GameController::isPressed(state)) {
				resetGame();
			}
		}
	};
	controller.addInputActionForKey(startAction);

	ButtonAction leftKeyAction;
	leftKeyAction.key = GameController::leftKey();
	leftKeyAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == IN_FALLING || mGameState == IN_BEGINNING) {
			if (GameController::isPressed(state)) {
				mGameState = IN_FALLING;
				//mCurrentFallingBlock.setVelocity(INITIAL_FALL_VELOCITY);
				if (mCurrentFallingBlock->getLeftPoint()->GetX() > mCurrentFallingBlock->getSquareSize().GetX()) {
					mCurrentFallingBlock->moveBy(Vec2D(-(mCurrentFallingBlock->getSquareSize().GetX()), 0));
					//mCurrentFallingBlock->moveBy(Vec2D(0, (mCurrentFallingBlock->getSquareSize().GetY())));
				}
			}
		}
	};
	controller.addInputActionForKey(leftKeyAction);

	ButtonAction rightKeyAction;
	rightKeyAction.key = GameController::rightKey();
	rightKeyAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == IN_FALLING || mGameState == IN_BEGINNING) {
			if (GameController::isPressed(state)) {
				mGameState = IN_FALLING;
				//mCurrentFallingBlock.setVelocity(INITIAL_FALL_VELOCITY);
				if (mCurrentFallingBlock->getRightPoint()->GetX() < App::singleton().width()) {
					mCurrentFallingBlock->moveBy(Vec2D(mCurrentFallingBlock->getSquareSize().GetX(), 0));
					//mCurrentFallingBlock->moveBy(Vec2D(0, (mCurrentFallingBlock->getSquareSize().GetY())));
				}
			}
		}
	};
	controller.addInputActionForKey(rightKeyAction);

	ButtonAction downKeyAction;
	downKeyAction.key = GameController::downKey();
	downKeyAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == IN_FALLING || mGameState == IN_BEGINNING) {
			if (GameController::isPressed(state)) {
				mGameState = IN_FALLING;
				while (true) {
					if (!mGameField.isSquaresBelowFull(mCurrentFallingBlock)
							&& mCurrentFallingBlock->getBottomPoint()->GetY()
									< App::singleton().height() - (mCurrentFallingBlock->getSquareSize().GetY())) {
						mCurrentFallingBlock->moveBy(INITIAL_FALL_VELOCITY);
					} else {
						break;
					}
				}
			}
		}
	};
	controller.addInputActionForKey(downKeyAction);

	ButtonAction upKeyAction;
	upKeyAction.key = GameController::upKey();
	upKeyAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == IN_FALLING || mGameState == IN_BEGINNING) {
			if (GameController::isPressed(state)) {
				mGameState = IN_FALLING;
				mCurrentFallingBlock->rotate();
			}
		}
	};
	controller.addInputActionForKey(upKeyAction);

	ButtonAction backAction;
	backAction.key = GameController::cancelKey();
	backAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == IN_BEGINNING || mGameState == IN_GAME_END || mGameState == IN_FALLING) {
			if (GameController::isPressed(state)) {
				App::singleton().popScene();
			}
		}
	};
	controller.addInputActionForKey(backAction);
}

void Tetris::update(uint32_t dt) {
	if (mGameState == IN_FALLING) {
		if (!mGameField.isSquaresBelowFull(mCurrentFallingBlock)
				&& mCurrentFallingBlock->getBottomPoint()->GetY()
						< App::singleton().height() - (mCurrentFallingBlock->getSquareSize().GetY())) {
			uint64_t tick = SDL_GetTicks();
			if (tick % 1000 < 10) {
				mCurrentFallingBlock->moveBy(INITIAL_FALL_VELOCITY);
			}
			//std::cout <<"tick:" << tick <<  " mod 1000:" << tick%1000 << " Time:" << SDL_GetTicks()/1000.0f << std::endl;
		} else if (mCurrentFallingBlock->getTopPoint()->GetY() > 0) {
			//std::cout << "2!" << std::endl;
			mCurrentFallingBlock->lock();
			mGameField.addTetromino(mCurrentFallingBlock);
			mGameField.addFills(mCurrentFallingBlock);
			mCurrentFallingBlock = getRandTetromino();
		} else {
			std::cout << "3!" << std::endl;
			mGameState = IN_GAME_END;
			resetGame();
		}
	}
}

void Tetris::draw(Screen &screen) {
	screen.draw(AARectangle(Vec2D(0.0f, 0.0f), App::singleton().width(), App::singleton().height()), Color::white());
	mCurrentFallingBlock->draw(screen);
	float width = App::singleton().width() / 10.0f;
	mGameField.drawField(screen);
	float height = App::singleton().height() / 20.0f;
	for (int i = 0; i < 11; i++) {
		screen.draw(Line2D(Vec2D(width * i, 0.0f), Vec2D(width * i, App::singleton().height())),
				Color(255, 255, 255, 255));
	}
	for (int i = 0; i < 21; i++) {
		screen.draw(Line2D(Vec2D(0.0f, height * i), Vec2D(App::singleton().width(), height * i)),
				Color(255, 255, 255, 255));
	}

}

const std::string& Tetris::getName() const {
	static std::string name = "Tetris!";
	return name;
}

void Tetris::resetGame() {
	mGameState = IN_FALLING;
	mGameField.clear();
	mCurrentFallingBlock = getRandTetromino();
	std::cout << mCurrentFallingBlock->getTopPoint()->GetX() << std::endl;
	mCurrentFallingBlock->setVelocity(INITIAL_FALL_VELOCITY);
	mGameField.addTetromino(mCurrentFallingBlock);
}
std::shared_ptr<Tetromino> Tetris::getRandTetromino() {
	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(0, 6);
	int randomNumber = uniform_dist(e1);
	switch (randomNumber) {
	case 0: {
		std::cout << "I " << std::endl;
		std::shared_ptr<TetrominoI> i(new TetrominoI());
		//static TetrominoI i;
		//return i;
		return i;
		//delete &i;
		std::cout << "break" << std::endl;
		break;
	}
	case 1: {
		std::cout << "J " << std::endl;
		std::shared_ptr<TetrominoJ> j(new TetrominoJ());
		return j;
		//static TetrominoS j;
		//return i;
		//mCurrentFallingBlock =
		//return std::make_unique<TetrominoJ>(j);
		//delete &j;
		std::cout << "break" << std::endl;
		break;
	}
	case 2: {
		std::cout << "L " << std::endl;
		std::shared_ptr<TetrominoL> l(new TetrominoL());
		return l;
		//delete &l;
		std::cout << "break" << std::endl;
		break;
	}
	case 3: {
		std::cout << "O " << std::endl;
		std::shared_ptr<TetrominoO> o(new TetrominoO());
		return o;
		//delete &o;
		std::cout << "break" << std::endl;
		break;
	}
	case 4: {
		std::cout << "S " << std::endl;
		std::shared_ptr<TetrominoS> s(new TetrominoS());
		return s;
		//delete &s;
		std::cout << "break" << std::endl;
		break;
	}
	case 5: {
		std::cout << "T " << std::endl;
		std::shared_ptr<TetrominoT> t(new TetrominoT());
		return t;
		//delete &t;
		std::cout << "break" << std::endl;
		break;
	}
	case 6: {
		std::cout << "Z " << std::endl;
		std::shared_ptr<TetrominoZ> z(new TetrominoZ());
		return z;
		//delete &z;
		std::cout << "break" << std::endl;
		break;
	}
	default: {
		std::cout << "I " << std::endl;
		std::shared_ptr<TetrominoI> i2(new TetrominoI());
		return i2;
		//mCurrentFallingBlock = std::make_unique<TetrominoI>();
		//delete &i2;
		std::cout << "break" << std::endl;
		break;
	}
	}
}
void Tetris::setToClearingState() {
}

bool Tetris::isGameOver() const {
	return false;
}

