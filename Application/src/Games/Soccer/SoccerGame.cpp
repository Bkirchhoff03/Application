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
const std::string READ_STR = "Ready!";
const std::string GAME_END_STR = "Game Over:(";
const uint32_t GAME_STARTING_TIME = 150;
}

void SoccerGame::init(GameController &controller) {
	mTimer = 0;
	mPlayerSpriteSheet.load("SoccerSprites");
	mPlayer.init(mPlayerSpriteSheet, App::singleton().getBasePath() + "Assets\\Soccer_animations.txt", Vec2D::zero,
			PLAYER_MOVEMENT_SPEED, false);
	mTeamAgainst.init(App::singleton().getBasePath() + "Assets\\Soccer_level.txt", &mPlayerSpriteSheet);

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
			if (defender.canChangeDirection() || (defenderAI.wantsToLeaveZone() && direction != PLAYER_MOVEMENT_NONE)) {
				if (direction != defender.getMovementDirection()) {
					defender.setMovementDirection(direction);
					defender.lockCanChangeDirection();
				}
			}

			defender.update(dt);
			if (!defender.isRealeasedFromZone() && mPlayer.getBoundingBox().intersects(defender.zoneBoundingBox())) {
				defender.setStateToDefending();
			}
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
	screen.draw(mPlayer.getBoundingBox(), Color::pink(), true, Color::pink());
	for (Defender &defender : mDefenders) {
		defender.draw(screen);
	}
	for (DefenderAI defenderAi : mDefenderAI) {
		defenderAi.draw(screen);
	}
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
	if (mPressedDirection == PLAYER_MOVEMENT_NONE) {
		mPlayer.stop();
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
	const Vec2D GK_SCATTER_POS = Vec2D(App::singleton().width() - 24, 0);
	const Vec2D LB_SCATTER_POS = Vec2D(App::singleton().width(), App::singleton().height());
	const Vec2D CB_SCATTER_POS = Vec2D(24, 0);
	const Vec2D RB_SCATTER_POS = Vec2D(0, App::singleton().height());
	const Vec2D CDM_SCATTER_POS = Vec2D(0, App::singleton().height());
	mDefenders.resize(NUM_DEFENDERS);
	mDefenderAI.resize(NUM_DEFENDERS);
	Size bounds = mTeamAgainst.getBounds();
	Defender Gk;
	Vec2D gkSpawn = mTeamAgainst.getDefenderSpawnPoints()[GOALKEEPER];
	/*AARectangle gkZone = AARectangle(Vec2D(gkSpawn.GetX() - 30 + bounds.width, gkSpawn.GetY() - 22 + bounds.height), 96,
			50);*/
	Gk.init(mPlayerSpriteSheet, App::singleton().getBasePath() + "Assets/Soccer_animations.txt", gkSpawn,
			DEFENDER_MOVEMENT_SPEED, true, Color::red());
	Gk.setMovementDirection(PLAYER_MOVEMENT_LEFT);
	mDefenders[GOALKEEPER] = Gk;
	auto GkAI = DefenderAI();
	GkAI.init(mDefenders[GOALKEEPER], Gk.getBoundingBox().getWidth(), GK_SCATTER_POS,
			mTeamAgainst.getDefenderSpawnPoints()[GOALKEEPER], mTeamAgainst.getDefenderSpawnPoints()[GOALKEEPER],
			GOALKEEPER);
	//GkAI.setZone(gkZone);
	mDefenderAI[GOALKEEPER] = GkAI;

	Defender lb;
	Vec2D lbSpawn = mTeamAgainst.getDefenderSpawnPoints()[LEFT_BACK];
	/*AARectangle lbZone = AARectangle(Vec2D(gkZone.getBottomRightPoint().GetX(), gkZone.getTopLeftPoint().GetY()), 30,
			160);*/
	lb.init(mPlayerSpriteSheet, App::singleton().getBasePath() + "Assets/Soccer_animations.txt", lbSpawn,
			DEFENDER_MOVEMENT_SPEED, true, Color::yellow());
	lb.setMovementDirection(PLAYER_MOVEMENT_LEFT);
	mDefenders[LEFT_BACK] = lb;
	auto lbAI = DefenderAI();
	lbAI.init(mDefenders[LEFT_BACK], lb.getBoundingBox().getWidth(), LB_SCATTER_POS,
			mTeamAgainst.getDefenderSpawnPoints()[LEFT_BACK], mTeamAgainst.getDefenderSpawnPoints()[LEFT_BACK],
			LEFT_BACK);
	//lbAI.setZone(lbZone);
	mDefenderAI[LEFT_BACK] = lbAI;

	Defender cb;
	Vec2D cbSpawn = mTeamAgainst.getDefenderSpawnPoints()[CENTER_BACK];
	/*AARectangle cbZone = AARectangle(Vec2D(gkZone.getTopLeftPoint().GetX(), gkZone.getBottomRightPoint().GetY()), 94,
			113);*/
	cb.init(mPlayerSpriteSheet, App::singleton().getBasePath() + "Assets/Soccer_animations.txt", cbSpawn,
			DEFENDER_MOVEMENT_SPEED, true, Color::orange());
	cb.setMovementDirection(PLAYER_MOVEMENT_LEFT);
	mDefenders[CENTER_BACK] = cb;
	auto cbAI = DefenderAI();
	cbAI.init(mDefenders[CENTER_BACK], cb.getBoundingBox().getWidth(), CB_SCATTER_POS,
			mTeamAgainst.getDefenderSpawnPoints()[CENTER_BACK], mTeamAgainst.getDefenderSpawnPoints()[CENTER_BACK],
			CENTER_BACK);
	//cbAI.setZone(cbZone);
	mDefenderAI[CENTER_BACK] = cbAI;

	Vec2D rbSpawn = mTeamAgainst.getDefenderSpawnPoints()[RIGHT_BACK];
	//AARectangle rbZone = AARectangle(Vec2D(rbSpawn.GetX(), gkZone.getTopLeftPoint().GetY()), 30, 160);
	Defender rb;
	rb.init(mPlayerSpriteSheet, App::singleton().getBasePath() + "Assets/Soccer_animations.txt", rbSpawn,
			DEFENDER_MOVEMENT_SPEED, true, Color::green());
	rb.setMovementDirection(PLAYER_MOVEMENT_LEFT);
	mDefenders[RIGHT_BACK] = rb;
	auto rbAI = DefenderAI();
	rbAI.init(mDefenders[RIGHT_BACK], rb.getBoundingBox().getWidth(), RB_SCATTER_POS,
			mTeamAgainst.getDefenderSpawnPoints()[RIGHT_BACK], mTeamAgainst.getDefenderSpawnPoints()[RIGHT_BACK],
			RIGHT_BACK);
	//rbAI.setZone(rbZone);
	mDefenderAI[RIGHT_BACK] = rbAI;

	Defender cdm;
	Vec2D cdmSpawn = mTeamAgainst.getDefenderSpawnPoints()[CENTER_DEFENSIVE_MIDFIELDER];
	/*AARectangle cdmZone = AARectangle(Vec2D(rbZone.getTopLeftPoint().GetX(), rbZone.getBottomRightPoint().GetY()), 160,
			60);*/
	cdm.init(mPlayerSpriteSheet, App::singleton().getBasePath() + "Assets/Soccer_animations.txt", cdmSpawn,
			DEFENDER_MOVEMENT_SPEED, true, Color::blue());
	cdm.setMovementDirection(PLAYER_MOVEMENT_LEFT);
	mDefenders[CENTER_DEFENSIVE_MIDFIELDER] = cdm;
	auto cdmAI = DefenderAI();
	cdmAI.init(mDefenders[CENTER_DEFENSIVE_MIDFIELDER], cdm.getBoundingBox().getWidth(), CDM_SCATTER_POS,
			mTeamAgainst.getDefenderSpawnPoints()[CENTER_DEFENSIVE_MIDFIELDER],
			mTeamAgainst.getDefenderSpawnPoints()[CENTER_DEFENSIVE_MIDFIELDER], CENTER_DEFENSIVE_MIDFIELDER);
	//cdmAI.setZone(cdmZone);
	mDefenderAI[CENTER_DEFENSIVE_MIDFIELDER] = cdmAI;
	for (size_t i = 0; i < NUM_DEFENDERS; ++i) {
		mDefenders[i].setDefenderDelegate(mDefenderAI[i]);
	}
	mTeamAgainst.makeDefenderZones(mDefenders, mDefenderAI);
}
