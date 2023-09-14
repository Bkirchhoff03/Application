/*
 * PacmanStartScene.h
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#ifndef SCENES_PACMAN_PACMANSTARTSCENE_H_
#define SCENES_PACMAN_PACMANSTARTSCENE_H_

#include "ButtonOptionsScene.h"

class PacmanStartScene: public ButtonOptionsScene {
public:
	PacmanStartScene();
	void init() override;
	void update(uint32_t dt) override;
	void draw(Screen &theScreen) override;
	const std::string& getSceneName() const override;
private:
};

#endif /* SCENES_PACMAN_PACMANSTARTSCENE_H_ */
