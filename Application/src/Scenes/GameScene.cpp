/*
 * GameScene.cpp
 *
 *  Created on: Jul 13, 2023
 *      Author: nebk2
 */
#include "GameScene.h"

GameScene::GameScene(std::unique_ptr<Game> optrGame): mGame(std::move(optrGame)){

}

void GameScene::init(){
	mGame->init(mGameController);
}

void GameScene::update(uint32_t dt){
	mGame->update(dt);
}

void GameScene::draw(Screen &screen){
	mGame->draw(screen);
}

const std::string& GameScene::getSceneName() const{
	return mGame->getName();
}


