/*
 * BreakOut.cpp
 *
 *  Created on: Jul 14, 2023
 *      Author: nebk2
 */

#include "BreakOut.h"
#include "GameController.h"
#include "App.h"
#include "Circle.h"
#include <iostream>

/*
 * Paddle
 * 		-Can move side to side (by the user)
 * 		-Stops at the edge of the screen
 * 		-Width and Height
 * 		-Bounces the ball
 * 			-Bounces the ball differently based on the position that was hit on the paddle
 *
 * 			---======---
 * 			+----------+
 * 			|          |
 * 			+----------+
 *
 * 	Ball
 * 		-Bounces off of the walls and the paddle and the blocks
 * 		-Width and Height
 * 		-Velocity position
 *
 * 	Block
 * 		-part of the level
 * 		-Has HP - One hit by the ball is -1 HP
 * 		-Color and position, Width and Height (AARectangle)
 * 		-Bounces the ball
 *
 * 	Level
 * 		-Container that holds all the blocks
 * 		-Should be able to load a level from the levels.txt file
 * 		-Handle collisions of the blocks?
 * 		-contain the walls/edges of the screens
 * 		-Reset the level
 *
 * 	Game
 * 		-Contains all of the objects (Paddle, Ball, Levels)
 * 		-Player which has 3 lives
 * 		-States - Serve the ball, In Game, Game Over
 * 		-Reset the game
 *
 */

void BreakOut::init(GameController &controller) {
	controller.clearAll();
	resetGame();
	ButtonAction serveAction;
	serveAction.key = GameController::actionKey();
	serveAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == IN_SERVE) {
			if (GameController::isPressed(state)) {
				mGameState = IN_PLAY;

				if (mPaddle.isMovingLeft()) {
					mBall.setVelocity(Vec2D(-INITIAL_BALL_SPEED, -INITIAL_BALL_SPEED));
				} else {
					mBall.setVelocity(Vec2D(INITIAL_BALL_SPEED, -INITIAL_BALL_SPEED));
				}
			}
		} else if (mGameState == IN_GAME_OVER) {
			if (GameController::isPressed(state)) {
				resetGame();
			}
		}
	};
	controller.addInputActionForKey(serveAction);

	ButtonAction leftKeyAction;
	leftKeyAction.key = GameController::leftKey();
	leftKeyAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == IN_PLAY || mGameState == IN_SERVE) {
			if (GameController::isPressed(state)) {
				mPaddle.setMovementDirection(PaddleDirection::LEFT);
			} else {
				mPaddle.unsetMovementDirection(PaddleDirection::LEFT);
			}
		}
	};
	controller.addInputActionForKey(leftKeyAction);

	ButtonAction rightKeyAction;
	rightKeyAction.key = GameController::rightKey();
	rightKeyAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == IN_PLAY || mGameState == IN_SERVE) {
			if (GameController::isPressed(state)) {
				mPaddle.setMovementDirection(PaddleDirection::RIGHT);
			} else {
				mPaddle.unsetMovementDirection(PaddleDirection::RIGHT);
			}
		}
	};
	controller.addInputActionForKey(rightKeyAction);

	ButtonAction backAction;
	backAction.key = GameController::cancelKey();
	backAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == IN_SERVE || mGameState == IN_GAME_OVER) {
			if (GameController::isPressed(state)) {
				App::singleton().popScene();
			}
		}
	};
	controller.addInputActionForKey(backAction);
}
void BreakOut::update(uint32_t dt) {
	if (mGameState == IN_SERVE) {
		mPaddle.update(dt, mBall);
		setToServeState();
	} else if (mGameState == IN_PLAY) {

		mBall.update(dt);
		mPaddle.update(dt, mBall);

		BoundaryEdge edge;
		if (mPaddle.bounce(mBall)) {
			return;
		}
		if (mLevelBoundary.hasCollided(mBall, edge)) {
			mBall.bounce(edge);
			return;
		}
		getCurrentLevel().update(dt, mBall);
		if (isBallPassedCutoffY()) {
			reduceLifeByOne();
			if (!isGameOver()) {
				setToServeState();
			} else {
				mGameState = IN_GAME_OVER;
			}
		} else if (getCurrentLevel().isLevelComplete()) {
			mCurrentLevel = (mCurrentLevel + 1) % mLevels.size();
			resetGame(mCurrentLevel);
		}
	}
}
void BreakOut::draw(Screen &screen) {
	mBall.draw(screen);
	mPaddle.draw(screen);
	getCurrentLevel().draw(screen);
	screen.draw(mLevelBoundary.getAARectangle(), Color::white());

	Circle lifeCircle = { Vec2D(7, App::singleton().height() - 10), 5 };

	for (int i = 0; i < mLives; ++i) {
		screen.draw(lifeCircle, Color::red(), true, Color::red());
		lifeCircle.moveBy(Vec2D(17, 0));
	}
}
const std::string& BreakOut::getName() const {
	static std::string name = "Break Out!";
	return name;
}

void BreakOut::resetGame(size_t toLevel) {
	mLevels = BreakoutGameLevel::loadLevelsFromFile(App::getBasePath() + "Assets/BreakoutLevels.txt");
	mYCutoff = App::singleton().height() - 2 * Paddle::PADDLE_HEIGHT;
	mLives = NUM_LIVES;
	mCurrentLevel = toLevel;
	AARectangle paddleRect = { Vec2D(App::singleton().width() / 2 - Paddle::PADDLE_WIDTH / 2,
			App::singleton().height() - 3 * Paddle::PADDLE_HEIGHT), Paddle::PADDLE_WIDTH, Paddle::PADDLE_HEIGHT };
	AARectangle levelBoundary = { Vec2D::zero, App::singleton().width(), App::singleton().height() };
	mLevelBoundary = { levelBoundary };

	mPaddle.init(paddleRect, levelBoundary);
	mBall.moveTo(Vec2D(App::singleton().width() / 2, App::singleton().height() * 0.75f));

	setToServeState();
}
void BreakOut::setToServeState() {
	mGameState = IN_SERVE;
	mBall.stop();

	mBall.moveTo(
			Vec2D(mPaddle.getAARectangle().getCenterPoint().GetX(),
					mPaddle.getAARectangle().getTopLeftPoint().GetY() - mBall.getRadius()));
}

bool BreakOut::isBallPassedCutoffY() const {
	return mBall.getPosition().GetY() > mYCutoff;
}
void BreakOut::reduceLifeByOne() {
	if (mLives >= 0) {
		--mLives;
	}
}
