/*
 * Block.cpp
 *
 *  Created on: Jul 16, 2023
 *      Author: nebk2
 */

#include "Block.h"
#include "Screen.h"
#include "Ball.h"

Block::Block():mOutlineColor(Color::white()), mFillColor(Color::white()), mHp(1){

}

void Block::init(const AARectangle &rect, int hp, const Color &outlineColor,
		const Color &fillColor) {
	Excluder::init(rect);
	mHp = hp;
	mOutlineColor = outlineColor;
	mFillColor = fillColor;
}

void Block::draw(Screen &screen) {
	screen.draw(getAARectangle(), mOutlineColor, true, mFillColor);
}

void Block::bounce(Ball& ball, const BoundaryEdge& edge) {
	ball.bounce(edge);
}

void Block::reduceHP() {
	if(mHp > 0){
		--mHp;
	}
}

