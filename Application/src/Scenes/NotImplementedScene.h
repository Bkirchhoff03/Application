/*
 * NotImplementedScene.h
 *
 *  Created on: Aug 10, 2023
 *      Author: nebk2
 */

#ifndef SCENES_NOTIMPLEMENTEDSCENE_H_
#define SCENES_NOTIMPLEMENTEDSCENE_H_

#include "Scene.h"
class Screen;
class NotImplementedScene: public Scene {
public:
	virtual void init() override;
	virtual void update(uint32_t dt) override;
	virtual void draw(Screen &theScreen) override;
	virtual const std::string& getSceneName() const override;

};

#endif /* SCENES_NOTIMPLEMENTEDSCENE_H_ */
