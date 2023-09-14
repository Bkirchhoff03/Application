/*
 * PacmanGame.cpp
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#include "PacmanGame.h"
#include "App.h"
#include "PacmanGameUtils.h"

/*
 * -Pacman
 * 		-eat ghost if at super pellet
 * 		-eat pellets
 * 		-move in 344 directions by player input
 * 		-animates
 * 		-can be eaten by ghosts
 * 		-has lives
 * 		-has score
 * 	-ghost
 * 		-eat pacman if not venerable
 * 		-move in 4 directions
 * 		-animates
 * 		-has different states (alive, venerable, venerable ending, dead)
 * 		-has AI
 * 		-has a specific spot on the level where it starts
 * 		-has a score value when eaten
 * 	-AI
 * 		-each ghost has a specific personality
 * 			-chase (Blinky red)
 * 			-cut off/ambush(Pinky)
 * 			-pinser attack with Pinky (inky blue)
 * 			-wander (Clyde orange)
 * 		-scatter mode
 * 	-Level
 * 		-contains all the walls (maze)
 * 		-contains pellets
 * 		-wraps pacman/ghosts at specific locations
 * 		-gate the releases the ghosts
 * 		-single background image
 * 	-pellet
 * 		-super pellet - turns ghosts to venerable state if eaten by pacman
 * 		-has a specific score value
 * 	-Special items
 * 		-Different ones appear with differing score values
 * 	-UI
 * 		-Score
 * 		-Lives left
 * 		-Game over/ready label
 * 	-Game
 * 		-Level
 * 		-Pacman
 * 		-ghosts
 * 		-states
 * 			-level starting
 * 			-play game
 * 			-lost life
 * 			-game over
 *
 *
 */

namespace {
const std::string SCORE_STR = "Score ";
const std::string PACMAN_LIFE_SPRITE_NAME = "pac_man_left_idle";
const size_t MAX_NUM_LIVES = 3;
const uint32_t RELEASE_GHOST_TIME = 5000;
const std::string READ_STR = "Ready!";
const std::string GAME_OVER_STR = "Game Over:(";
const uint32_t LEVEL_STARTING_TIME = 3000;
}

void PacmanGame::init(GameController &controller) {
	mReleaseGhostTimer = 0;
	mPacmanSpriteSheet.load("PacmanSprites");
	mPacman.init(mPacmanSpriteSheet, App::singleton().getBasePath() + "Assets/Pacman_animations.txt", Vec2D::zero,
			PACMAN_MOVEMENT_SPEED, false);
	mLevel.init(App::singleton().getBasePath() + "Assets/Pacman_level.txt", &mPacmanSpriteSheet);

	mStringRect = AARectangle(Vec2D(0, mLevel.getInGameTextYPos()), App::singleton().width(),
			mPacman.getBoundingBox().getHeight());

	setupGhosts();
	resetGame();

	ButtonAction leftAction;
	leftAction.key = GameController::leftKey();
	leftAction.action = [this](uint32_t dt, InputState state) {
		handleGameControllerState(dt, state, PACMAN_MOVEMENT_LEFT);
	};
	controller.addInputActionForKey(leftAction);

	ButtonAction rightAction;
	rightAction.key = GameController::rightKey();
	rightAction.action = [this](uint32_t dt, InputState state) {
		handleGameControllerState(dt, state, PACMAN_MOVEMENT_RIGHT);
	};
	controller.addInputActionForKey(rightAction);

	ButtonAction upAction;
	upAction.key = GameController::upKey();
	upAction.action = [this](uint32_t dt, InputState state) {
		handleGameControllerState(dt, state, PACMAN_MOVEMENT_UP);
	};
	controller.addInputActionForKey(upAction);

	ButtonAction downAction;
	downAction.key = GameController::downKey();
	downAction.action = [this](uint32_t dt, InputState state) {
		handleGameControllerState(dt, state, PACMAN_MOVEMENT_DOWN);
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
void PacmanGame::update(uint32_t dt) {

	if (mGameState == LEVEL_STARTING) {
		mPacman.resetGhostEatenMultiplier();
		mLevelStartingTimer += dt;

		if (mLevelStartingTimer >= LEVEL_STARTING_TIME) {
			mLevelStartingTimer = 0;
			mGameState = PLAY_GAME;
		}
	} else if (mGameState == PLAY_GAME) {
		updatePacmanMovement();
		mPacman.update(dt);

		mReleaseGhostTimer += dt;

		for (size_t i = 0; i < NUM_GHOSTS; ++i) {

			Ghost &ghost = mGhosts[i];
			GhostAI &ghostAI = mGhostAI[i];
			if (mReleaseGhostTimer >= RELEASE_GHOST_TIME && ghostAI.isInPen() && !ghost.isRealeased()) {
				mReleaseGhostTimer = 0;
				ghost.releasedFromPen();
			}

			auto direction = ghostAI.update(dt, mPacman, mLevel, mGhosts);
			if ((ghost.isRealeased() && ghost.canChangeDirection())
					|| (ghost.isRealeased() && ghostAI.wantsToLeavePen() && direction != PACMAN_MOVEMENT_NONE)) {
				if (direction != ghost.getMovementDirection()) {
					ghost.setMovementDirection(direction);
					ghost.lockCanChangeDirection();
				}
			}

			ghost.update(dt);

			if (ghost.isVulnerable() && mPacman.getEatingBoundingBox().intersects(ghost.getBoundingBox())) {
				ghost.eatenByPacman();
				mPacman.ateGhost(ghost.getPoints());
			} else if (ghost.isAlive() && ghost.getEatingBoundingBox().intersects(mPacman.getBoundingBox())) {
				mNumLives--;
				mGameState = LOST_LIFE;
				mPacman.eatenByGhost();
				mPressedDirection = PACMAN_MOVEMENT_NONE;
				mPacman.setMovementDirection(PACMAN_MOVEMENT_NONE);
				return;
			}
		}

		mLevel.update(dt, mPacman, mGhosts, mGhostAI);

		if (mLevel.isLevelOver()) {
			mLevel.increaseLevel();
			resetLevel();
		}
	} else if (mGameState == LOST_LIFE) {
		mPacman.update(dt);
		if (mPacman.isFinishedAnimation()) {
			if (mNumLives >= 0) {
				resetLevel();
			} else {
				mGameState = GAME_OVER;
			}
		}
	}

}
void PacmanGame::draw(Screen &screen) {
	mLevel.draw(screen);
	mPacman.draw(screen);

	for (Ghost &ghost : mGhosts) {
		ghost.draw(screen);
	}

	/*for (GhostAI &ghostAI : mGhostAI) {
		ghostAI.draw(screen);
	 }*/
	const auto &font = App::singleton().getFont();

	Vec2D textDrawPosition;

	//Draw Score
	{
		Vec2D levelOffset = mLevel.getLayoutOffset();

		AARectangle highScoreRect = AARectangle(Vec2D(0, 4), App::singleton().width(), levelOffset.GetY());

		std::string scoreStr = std::to_string(mPacman.score());

		textDrawPosition = font.getDrawPosition(SCORE_STR + scoreStr, highScoreRect, BFXA_CENTER, BFYA_CENTER);

		screen.draw(font, SCORE_STR + scoreStr, textDrawPosition);
	}

	if (mGameState == LEVEL_STARTING) {
		textDrawPosition = font.getDrawPosition(READ_STR, mStringRect, BFXA_CENTER, BFYA_CENTER);
		screen.draw(font, READ_STR, textDrawPosition, Color::yellow());
	} else if (mGameState == GAME_OVER) {
		textDrawPosition = font.getDrawPosition(GAME_OVER_STR, mStringRect, BFXA_CENTER, BFYA_CENTER);
		screen.draw(font, GAME_OVER_STR, textDrawPosition, Color::yellow());

	}

	drawLives(screen);
}

void PacmanGame::drawLives(Screen &screen) {
	const uint32_t X_PAD = 1;

	auto sprite = mPacmanSpriteSheet.getSprite(PACMAN_LIFE_SPRITE_NAME);

	uint32_t xPos = X_PAD;
	for (int i = 0; i < mNumLives; ++i) {
		screen.draw(mPacmanSpriteSheet, PACMAN_LIFE_SPRITE_NAME,
				Vec2D(xPos, App::singleton().height() - sprite.height));
		xPos += X_PAD + sprite.width;
	}
}

const std::string& PacmanGame::getName() const {
	static std::string name = "!Pacman";
	return name;
}

void PacmanGame::resetGame() {
	mNumLives = MAX_NUM_LIVES;
	mPressedDirection = PACMAN_MOVEMENT_NONE;
	mPacman.resetScore();
	mLevel.resetToFirstLevel();
	resetLevel();
}

void PacmanGame::resetLevel() {
	mReleaseGhostTimer = 0;
	mGameState = LEVEL_STARTING;
	mPacman.moveTo(mLevel.getPacmanSpawnLocation());
	mPacman.resetToFirstAnimation();

	for (Ghost &ghost : mGhosts) {
		ghost.resetToFirstPosition();
	}
	mGhosts[BLINKY].releasedFromPen();
}

void PacmanGame::updatePacmanMovement() {
	PacmanMovement currentDirection = mPacman.getMovementDirection();
	if (mLevel.willCollide(mPacman.getBoundingBox(), currentDirection)) {
		mPacman.stop();
		return;
	}
	if (mPressedDirection != PACMAN_MOVEMENT_NONE && currentDirection != mPressedDirection) {
		if (!mLevel.willCollide(mPacman.getBoundingBox(), mPressedDirection)) {
			mPacman.setMovementDirection(mPressedDirection);
		}
	}
}
void PacmanGame::handleGameControllerState(uint32_t dt, InputState state, PacmanMovement direction) {
	if (GameController::isPressed(state)) {
		mPressedDirection = direction;
	} else if (GameController::isReleased(state) && mPressedDirection == direction) {
		mPressedDirection = PACMAN_MOVEMENT_NONE;
	}
}

void PacmanGame::setupGhosts() {
	const Vec2D BLINKY_SCATTER_POS = Vec2D(App::singleton().width() - 24, 0);
	const Vec2D INKY_SCATTER_POS = Vec2D(App::singleton().width(), App::singleton().height());
	const Vec2D PINKY_SCATTER_POS = Vec2D(24, 0);
	const Vec2D CLYDE_SCATTER_POS = Vec2D(0, App::singleton().height());

	mGhosts.resize(NUM_GHOSTS);
	mGhostAI.resize(NUM_GHOSTS);

	Ghost blinky;
	blinky.init(mPacmanSpriteSheet, App::singleton().getBasePath() + "Assets/Ghost_animations.txt",
			mLevel.getGhostSpawnPoints()[BLINKY], GHOST_MOVEMENT_SPEED, true, Color::red());
	blinky.setMovementDirection(PACMAN_MOVEMENT_LEFT);
	mGhosts[BLINKY] = blinky;

	auto blinkyAI = GhostAI();
	blinkyAI.init(mGhosts[BLINKY], blinky.getBoundingBox().getWidth(), BLINKY_SCATTER_POS,
			mLevel.getGhostSpawnPoints()[PINKY], mLevel.getGhostSpawnPoints()[BLINKY], BLINKY);

	mGhostAI[BLINKY] = blinkyAI;

	Ghost pinky;
	pinky.init(mPacmanSpriteSheet, App::singleton().getBasePath() + "Assets/Ghost_animations.txt",
			mLevel.getGhostSpawnPoints()[PINKY], GHOST_MOVEMENT_SPEED, true, Color::pink());
	pinky.setMovementDirection(PACMAN_MOVEMENT_DOWN);
	mGhosts[PINKY] = pinky;

	auto pinkyAI = GhostAI();
	pinkyAI.init(mGhosts[PINKY], pinky.getBoundingBox().getWidth(), PINKY_SCATTER_POS,
			mLevel.getGhostSpawnPoints()[PINKY], mLevel.getGhostSpawnPoints()[BLINKY], PINKY);

	mGhostAI[PINKY] = pinkyAI;

	Ghost inky;
	inky.init(mPacmanSpriteSheet, App::singleton().getBasePath() + "Assets/Ghost_animations.txt",
			mLevel.getGhostSpawnPoints()[INKY], GHOST_MOVEMENT_SPEED, true, Color::cyan());
	inky.setMovementDirection(PACMAN_MOVEMENT_UP);
	mGhosts[INKY] = inky;

	auto inkyAI = GhostAI();
	inkyAI.init(mGhosts[INKY], inky.getBoundingBox().getWidth(), INKY_SCATTER_POS, mLevel.getGhostSpawnPoints()[PINKY],
			mLevel.getGhostSpawnPoints()[BLINKY], INKY);

	mGhostAI[INKY] = inkyAI;

	Ghost clyde;
	clyde.init(mPacmanSpriteSheet, App::singleton().getBasePath() + "Assets/Ghost_animations.txt",
			mLevel.getGhostSpawnPoints()[CLYDE], GHOST_MOVEMENT_SPEED, true, Color::orange());
	clyde.setMovementDirection(PACMAN_MOVEMENT_UP);
	mGhosts[CLYDE] = clyde;

	auto clydeAI = GhostAI();
	clydeAI.init(mGhosts[CLYDE], clyde.getBoundingBox().getWidth(), CLYDE_SCATTER_POS,
			mLevel.getGhostSpawnPoints()[PINKY], mLevel.getGhostSpawnPoints()[BLINKY], CLYDE);

	mGhostAI[CLYDE] = clydeAI;

	for (size_t i = 0; i < NUM_GHOSTS; ++i) {
		mGhosts[i].setGhostDelegate(mGhostAI[i]);
	}
}
