/*
 * SoccerGame.cpp
 *
 *  Created on: Sep 28, 2023
 *      Author: nebk2
 */
#include "App.h"
#include "SoccerGame.h"
#include "TeamAgainst.h"

namespace {
const std::string SCORE_STR = "Score ";
const std::string PACMAN_LIFE_SPRITE_NAME = "pac_man_left_idle";
//const size_t MAX_NUM_LIVES = 3;
//const uint32_t RELEASE_GHOST_TIME = 5000;
const std::string READ_STR = "Ready!";
const std::string GAME_END_STR = "Game Over:(";
const uint32_t GAME_STARTING_TIME = 3000;
}

void SoccerGame::init(GameController &controller) {
	mTimer = 0;
	mPlayerSpriteSheet.load("PacmanSprites");
	mPlayer.init(mPlayerSpriteSheet, App::singleton().getBasePath() + "Assets/Pacman_animations.txt", Vec2D::zero,
			PLAYER_MOVEMENT_SPEED, false);
	mTeamAgainst.init(App::singleton().getBasePath() + "Assets/Pacman_level.txt", &mPlayerSpriteSheet);

	mStringRect = AARectangle(Vec2D(0, mTeamAgainst.getInGameTextYPos()), App::singleton().width(),
			mPlayer.getBoundingBox().getHeight());

	setupDefenders();
	resetGame();

	ButtonAction leftAction;
	leftAction.key = GameController::leftKey();
	leftAction.action = [this](uint32_t dt, InputState state) {
		handleGameControllerState(dt, state, PLAYER_MOVEMENT_LEFT);
	};
	controller.addInputActionForKey(leftAction);

	ButtonAction rightAction;
	rightAction.key = GameController::rightKey();
	rightAction.action = [this](uint32_t dt, InputState state) {
		handleGameControllerState(dt, state, PLAYER_MOVEMENT_RIGHT);
	};
	controller.addInputActionForKey(rightAction);

	ButtonAction upAction;
	upAction.key = GameController::upKey();
	upAction.action = [this](uint32_t dt, InputState state) {
		handleGameControllerState(dt, state, PLAYER_MOVEMENT_UP);
	};
	controller.addInputActionForKey(upAction);

	ButtonAction downAction;
	downAction.key = GameController::downKey();
	downAction.action = [this](uint32_t dt, InputState state) {
		handleGameControllerState(dt, state, PLAYER_MOVEMENT_DOWN);
	};
	controller.addInputActionForKey(downAction);

	ButtonAction backAction;
	backAction.key = GameController::cancelKey();
	backAction.action = [this](uint32_t dt, InputState state) {
		if (mGameState == GAME_OVER && GameController::isPressed(state)) {
			App::singleton().popScene();
		}
	};

	controller.addInputActionForKey(backAction);

}
void SoccerGame::update(uint32_t dt) {

	if (mGameState == GAME_STARTING) {
		//mPlayer.resetGhostEatenMultiplier();
		mLevelStartingTimer += dt;

		if (mLevelStartingTimer >= GAME_STARTING_TIME) {
			mLevelStartingTimer = 0;
			mGameState = IN_GAME;
		}
	} else if (mGameState == IN_GAME) {
		updatePlayerMovement();
		mPlayer.update(dt);

		//mReleaseGhostTimer += dt;

		for (size_t i = 0; i < NUM_DEFENDERS; ++i) {

			Defender &defender = mDefenders[i];
			DefenderAI &defenderAI = mDefenderAI[i];
			//if (mReleaseGhostTimer >= RELEASE_GHOST_TIME && ghostAI.isInPen() && !ghost.isRealeased()) {
			//mReleaseGhostTimer = 0;
			//ghost.releasedFromPen();
			//}

			auto direction = defenderAI.update(dt, mPlayer, mTeamAgainst, mDefenders);
			if ((defender.isRealeasedFromZone() && defender.canChangeDirection())
					|| (defender.isRealeasedFromZone() && defenderAI.wantsToLeaveZone()
							&& direction != PLAYER_MOVEMENT_NONE)) {
				if (direction != defender.getMovementDirection()) {
					defender.setMovementDirection(direction);
					defender.lockCanChangeDirection();
				}
			}

			defender.update(dt);
			if (defender.isWithBall() && mPlayer.getDribbleBoundingBox().intersects(defender.getBoundingBox())) {
				//auto random = srand(100);
				//if (random < defender.getTackleOdds()) {
				defender.ballLost();
				//}
			} else if (defender.isWithoutBall() && mPlayer.isWithBall()
					&& defender.getDribbleBoundingBox().intersects(mPlayer.getBoundingBox())) {
				srand(100);
				auto random = rand();

				if (random < defender.getTackleOdds()) {
					mPlayer.ballTakenByDefender();
				} else if (random < defender.getFoulOdds()) {
					callFoul(defender);
				}
				mPressedDirection = PLAYER_MOVEMENT_NONE;
				mPlayer.setMovementDirection(PLAYER_MOVEMENT_NONE);
				return;
			}
			//ADD BALL IN GOAL MECHANICS NOT PLAYER MECHANICS
			/*if (defender.isWithBall() && mPlayer.getDribbleBoundingBox().intersects(defender.getBoundingBox())) {
			 defender.scoredOnByPlayer();
			 mPlayer.addToScore(1);
			 } else if (defender.isWithoutBall()
			 && defender.getDribbleBoundingBox().intersects(mPlayer.getBoundingBox())) {
			 mNumGoalsFor--;
			 mGameState = SCORED_ON;
			 mPlayer.ballTakenByDefender();
			 mPressedDirection = PLAYER_MOVEMENT_NONE;
			 mPlayer.setMovementDirection(PLAYER_MOVEMENT_NONE);
			 return;
			 }*/
		}

		mTeamAgainst.update(dt, mPlayer, mDefenders, mDefenderAI);

		if (mTeamAgainst.isGameOver()) {
			mTeamAgainst.increaseLevel();
			resetGame();
		}
	} else if (mGameState == SCORED_ON) {
		mPlayer.update(dt);
		if (mPlayer.isFinishedAnimation()) {
			if (mTimer >= 0) {
				resetGameAfterScore();
			} else {
				mGameState = GAME_OVER;
			}
		}
	}
}

void SoccerGame::draw(Screen &screen) {
	mTeamAgainst.draw(screen);
	mPlayer.draw(screen);

	for (Defender &defender : mDefenders) {
		defender.draw(screen);
	}

	/*for (GhostAI &ghostAI : mGhostAI) {
	 ghostAI.draw(screen);
	 }*/
	const auto &font = App::singleton().getFont();

	Vec2D textDrawPosition;

	//Draw Score
	{
		Vec2D levelOffset = mTeamAgainst.getLayoutOffset();

		AARectangle highScoreRect = AARectangle(Vec2D(0, 4), App::singleton().width(), levelOffset.GetY());

		std::string scorePlayerStr = std::to_string(mNumGoalsFor);
		std::string scoreDefendersStr = std::to_string(mNumGoalsAgainst);

		textDrawPosition = font.getDrawPosition(SCORE_STR + scorePlayerStr + " - " + scoreDefendersStr, highScoreRect,
				BFXA_CENTER, BFYA_CENTER);

		screen.draw(font, SCORE_STR + scorePlayerStr + " - " + scoreDefendersStr, textDrawPosition);
	}

	if (mGameState == GAME_STARTING) {
		textDrawPosition = font.getDrawPosition(READ_STR, mStringRect, BFXA_CENTER, BFYA_CENTER);
		screen.draw(font, READ_STR, textDrawPosition, Color::yellow());
	} else if (mGameState == GAME_OVER) {
		textDrawPosition = font.getDrawPosition(GAME_END_STR, mStringRect, BFXA_CENTER, BFYA_CENTER);
		screen.draw(font, GAME_END_STR, textDrawPosition, Color::yellow());
	}
}
const std::string& SoccerGame::getName() const {
	static std::string name = "Soccer!";
	return name;
}
void SoccerGame::callFoul(SoccerPlayer player) {
	mGameState = GAME_STOPPED;
	//ADD BALL MECHANICS
	mPlayer.giveBall();
}
void SoccerGame::resetGameAfterScore() {
	//mReleaseGhostTimer = 0;
	//mGameState = LEVEL_STARTING;
	mPlayer.moveTo(mTeamAgainst.getPlayerSpawnLocation());
	mPlayer.resetToFirstAnimation();

	for (Defender &defender : mDefenders) {
		defender.resetToFirstPosition();
	}
	//mDefenders[CENTER_BACK].releasedFromPen();
}
void SoccerGame::updatePlayerMovement() {
	PlayerMovement currentDirection = mPlayer.getMovementDirection();
	if (mTeamAgainst.willCollide(mPlayer.getBoundingBox(), currentDirection)) {
		mPlayer.stop();
		return;
	}
	if (mPressedDirection != PLAYER_MOVEMENT_NONE && currentDirection != mPressedDirection) {
		if (!mTeamAgainst.willCollide(mPlayer.getBoundingBox(), mPressedDirection)) {
			mPlayer.setMovementDirection(mPressedDirection);
		}
	}
}
void SoccerGame::handleGameControllerState(uint32_t dt, InputState state, PlayerMovement direction) {
	if (GameController::isPressed(state)) {
		mPressedDirection = direction;
	} else if (GameController::isReleased(state) && mPressedDirection == direction) {
		mPressedDirection = PLAYER_MOVEMENT_NONE;
	}
}
void SoccerGame::resetGame() {
	mGameState = GAME_STARTING;
	mPlayer.moveTo(mTeamAgainst.getPlayerSpawnLocation());
	mPlayer.resetToFirstAnimation();

	for (Defender &defender : mDefenders) {
		defender.resetToFirstPosition();
	}
	mDefenders[CENTER_DEFENSIVE_MIDFIELDER].releaseFromZone();
}
void SoccerGame::drawScore(Screen &screen) {
}
void SoccerGame::setupDefenders() {
}
