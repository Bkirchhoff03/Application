/*
 * Block.h
 *
 *  Created on: Jul 16, 2023
 *      Author: nebk2
 */

#ifndef GAMES_BREAKOUT_BLOCK_H_
#define GAMES_BREAKOUT_BLOCK_H_

#include "Excluder.h"
#include "Color.h"

class Ball;
class Screen;
struct BoundaryEdge;

class Block: public Excluder{
public:
	static const int UNBREAKABLE = -1;

	Block();
	void init(const AARectangle& rect, int hp, const Color& outlineColor, const Color& fillColor);
	void draw(Screen& screen);
	void bounce(Ball& ball, const BoundaryEdge& edge);

	void reduceHP();
	inline int getHP() const {return mHp;}
	inline bool isDestroyed() const {return mHp == 0;}
	inline const Color& getOutlineColor() const {return mOutlineColor;}
	inline const Color& getFillColor() const {return mFillColor;}
private:
	Color mOutlineColor;
	Color mFillColor;

	int mHp; //-1: means unbreakable
};



#endif /* GAMES_BREAKOUT_BLOCK_H_ */
