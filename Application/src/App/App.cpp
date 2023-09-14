/*
 * App.cpp
 *
 *  Created on: Jul 10, 2023
 *      Author: nebk2
 */

#include "App.h"
#include <SDL2/SDL.h>
#include <iostream>
#include "ArcadeScene.h"
#include <cassert>

App& App::singleton() {
	static App theApp;
	return theApp;
}

bool App::init(uint32_t width, uint32_t height, uint32_t mag) {
	if (!mFont.load("ArcadeFont")) {
		std::cout << "Could't load arcade font:(" << std::endl;
		return false;
	}
	mnoptrWindow = mScreen.init(width, height, mag);

	std::unique_ptr<ArcadeScene> arcadeScene = std::make_unique<ArcadeScene>();

	pushScene(std::move(arcadeScene));

	return mnoptrWindow != nullptr;
}

void App::run() {
	if (mnoptrWindow) {
		bool running = true;

		uint32_t lastTick = SDL_GetTicks();
		uint32_t currentTick = lastTick;

		uint32_t dt = 10;
		uint32_t accumulator = 0;

		mInputController.init([&running](uint32_t dt, InputState state) {
			running = false;
		});

		while (running) {
			currentTick = SDL_GetTicks();
			uint32_t frameTime = currentTick - lastTick;
			if (frameTime > 300) {
				frameTime = 300;
			}

			lastTick = currentTick;

			accumulator += frameTime;

			//input
			mInputController.update(dt);
			Scene *topScene = App::topScene();
			assert(topScene && "Why don't we have a scene?");

			if (topScene) {
				//Update
				//std::cout << "drawing top Scene!!" << std::endl;
				while (accumulator >= dt) {
					//update current scene by dt
					//std::cout << "dt: " << dt << std::endl;
					topScene->update(dt);
					accumulator -= dt;
					//dt++;
				}
				//render
				topScene->draw(mScreen);

			}

			mScreen.swapScreens();

			//std::cout << currentTick << " " << running << endl;
		}
	}
}

void App::pushScene(std::unique_ptr<Scene> scene) {
	assert(scene && "Don't push nullptr");
	if (scene) {
		scene->init();
		mInputController.setGameController(scene->getGameController());
		mSceneStack.emplace_back(std::move(scene));
		SDL_SetWindowTitle(mnoptrWindow, topScene()->getSceneName().c_str());
	}
}
void App::popScene() {
	if (mSceneStack.size() > 1) {
		mSceneStack.pop_back();
	}
	if (topScene()) {
		mInputController.setGameController(topScene()->getGameController());
		SDL_SetWindowTitle(mnoptrWindow, topScene()->getSceneName().c_str());
	}
}
Scene* App::topScene() {
	if (mSceneStack.empty()) {
		return nullptr;
	}

	return mSceneStack.back().get();
}

const std::string& App::getBasePath() {
	static std::string basePath = SDL_GetBasePath();
	return basePath;
}
