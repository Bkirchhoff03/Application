/*
 * Breakout.h
 *
 *  Created on: Jul 14, 2023
 *      Author: nebk2
 */

#ifndef GAMES_BREAKOUT_H_
#define GAMES_BREAKOUT_H_

#include "Game.h"
#include "Paddle.h"
#include "Ball.h"
#include "LevelBoundary.h"
#include "BreakoutGameLevel.h"
#include <vector>

enum BreakOutGameState{
	IN_PLAY = 0,
	IN_SERVE,
	IN_GAME_OVER
};

class BreakOut: public Game{
public:
	virtual void init(GameController& controller) override;
	virtual void update(uint32_t dt) override;
	virtual void draw(Screen& screen) override;
	virtual const std::string& getName() const override;
private:
	const int NUM_LIVES = 3;

	void resetGame(size_t toLevel = 0);

	BreakoutGameLevel& getCurrentLevel() {return mLevels[mCurrentLevel];}
	void setToServeState();

	bool isBallPassedCutoffY() const;
	void reduceLifeByOne();
	bool isGameOver() const {return mLives < 0;}

	const float INITIAL_BALL_SPEED = 100;
	const Vec2D INITIAL_BALL_VELOCITY = Vec2D(100, -100);
	Ball mBall;
	Paddle mPaddle;
	LevelBoundary mLevelBoundary;
	std::vector<BreakoutGameLevel> mLevels;
	size_t mCurrentLevel;
	BreakOutGameState mGameState;
	int mLives;
	float mYCutoff;
};




#endif /* GAMES_BREAKOUT_H_ */
