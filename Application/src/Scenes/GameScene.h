/*
 * GameScene.h
 *
 *  Created on: Jul 13, 2023
 *      Author: nebk2
 */

#ifndef SCENES_GAMESCENE_H_
#define SCENES_GAMESCENE_H_

#include "Scene.h"
#include "Game.h"
#include <memory>
class GameScene: public Scene{
public:
	GameScene(std::unique_ptr<Game> optrGame);
	virtual ~GameScene() {}
	virtual void init() override;
	virtual void update(uint32_t dt) override;
	virtual void draw(Screen& screen) override;

	virtual const std::string& getSceneName() const override;

private:
	std::unique_ptr<Game> mGame;
};



#endif /* SCENES_GAMESCENE_H_ */
