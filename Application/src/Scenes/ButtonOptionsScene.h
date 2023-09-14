/*
 * ButtonOptionsScene.h
 *
 *  Created on: Aug 10, 2023
 *      Author: nebk2
 */

#ifndef SCENES_BUTTONOPTIONSSCENE_H_
#define SCENES_BUTTONOPTIONSSCENE_H_

#include "Scene.h"
#include "Button.h"
#include <string>
#include <vector>

class Screen;

class ButtonOptionsScene: public Scene {
public:

	ButtonOptionsScene(const std::vector<std::string> &optionNames, const Color &textColor);

	virtual void init() override;
	virtual void update(uint32_t dt) override;
	virtual void draw(Screen &theScreen) override;

	void setButtonActions(const std::vector<Button::ButtonAction> &buttonActions);

private:

	void setNextButtonHighlighted();
	void setPreviousButtonHighlighted();
	void executeCurrentButtonAction();
	void highlightCurrentButton();

	std::vector<Button> mButtons;
	int mHighlightedOption;
};

#endif /* SCENES_BUTTONOPTIONSSCENE_H_ */
