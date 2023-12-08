/*
 * PacmanStartScene.h
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#ifndef SCENES_SOCCER_SOCCERSTARTSCENE_H_
#define SCENES_SOCCER_SOCCERSTARTSCENE_H_

#include "ButtonOptionsScene.h"

class SoccerStartScene: public ButtonOptionsScene {
public:
	SoccerStartScene();
	void init() override;
	void update(uint32_t dt) override;
	void draw(Screen &theScreen) override;
	const std::string& getSceneName() const override;
private:
};

#endif /* SCENES_SOCCER_SOCCERSTARTSCENE_H_ */
