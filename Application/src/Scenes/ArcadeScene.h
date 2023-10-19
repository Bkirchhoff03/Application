/*
 *
 * ArcadeScene.h
 *
 *  Created on: Jul 10, 2023
 *      Author: nebk2
 */

#ifndef SCENES_ARCADESCENE_H_
#define SCENES_ARCADESCENE_H_

#include "ButtonOptionsScene.h"
#include <memory>

enum eGame {
	TETRIS = 0, BREAKOUT, ASTEROIDS, PACMAN, SOCCER, NUM_GAMES
};

class Screen;

class ArcadeScene: public ButtonOptionsScene {
public:
	ArcadeScene();
	virtual void init() override;
	virtual void update(uint32_t dt) override;
	virtual void draw(Screen &theScreen) override;
	virtual const std::string& getSceneName() const override;
private:
	std::unique_ptr<Scene> getScene(eGame game);
	SpriteSheet mSpriteSheet;
};

#endif /* SCENES_ARCADESCENE_H_ */
