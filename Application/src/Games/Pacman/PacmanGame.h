/*
 * PacmanGame.h
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#ifndef GAMES_PACMAN_PACMANGAME_H_
#define GAMES_PACMAN_PACMANGAME_H_

#include "Game.h"
#include "PacmanLevel.h"
#include "Pacman.h"
#include "SpriteSheet.h"
#include "InputAction.h"
#include "Ghost.h"
#include "GhostAI.h"

enum PacmanGameState {
	LEVEL_STARTING = 0, PLAY_GAME, LOST_LIFE, GAME_OVER
};

class PacmanGame: public Game {
public:
	void init(GameController &controller) override;
	void update(uint32_t dt) override;
	void draw(Screen &screen) override;
	const std::string& getName() const override;
private:
	void resetGame();
	void updatePacmanMovement();
	void handleGameControllerState(uint32_t dt, InputState state, PacmanMovement direction);
	void resetLevel();
	void drawLives(Screen &screen);
	void setupGhosts();

	PacmanMovement mPressedDirection;
	SpriteSheet mPacmanSpriteSheet;
	Pacman mPacman;
	PacmanLevel mLevel;
	int mNumLives;
	std::vector<Ghost> mGhosts;
	std::vector<GhostAI> mGhostAI;
	uint32_t mReleaseGhostTimer;
	PacmanGameState mGameState;
	uint32_t mLevelStartingTimer;
	AARectangle mStringRect;
};

#endif /* GAMES_PACMAN_PACMANGAME_H_ */
