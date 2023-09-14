/*
 * Paddle.h
 *
 *  Created on: Jul 15, 2023
 *      Author: nebk2
 */

#ifndef GAMES_BREAKOUT_PADDLE_H_
#define GAMES_BREAKOUT_PADDLE_H_

#include "Excluder.h"
#include <stdint.h>
class Screen;
class Ball;
enum PaddleDirection{
	LEFT = 1 << 0,
	RIGHT = 1 << 1
};
class Paddle: public Excluder{
public:
	static const uint32_t PADDLE_WIDTH = 50;
	static const uint32_t PADDLE_HEIGHT = 10;

	void init(const AARectangle& rect, const AARectangle& boundary);

	void update(uint32_t dt, Ball& ball);

	void draw(Screen& screen);

	bool bounce(Ball& ball);

	inline bool isMovingLeft() const {return mDirection == PaddleDirection::LEFT;}
	inline bool isMovingRight() const {return mDirection == PaddleDirection::RIGHT;}

	//~00 = 11
	//0 | 1 = 1
	//0 & 1 = 0
	//1 & 1 = 1
	inline void setMovementDirection(PaddleDirection dir){mDirection |= dir;}
	inline void unsetMovementDirection(PaddleDirection dir) {mDirection &= ~dir;}
	inline void stopMovement() {mDirection = 0;}

private:
	uint32_t mDirection; //direction we're moving
	AARectangle mBoundary; //boundary its confined to
	const float VELOCITY = 100.0f; //pixels/sec'
	const float CORNER_BOUNCE_AMT = 0.2f;
};


#endif /* GAMES_BREAKOUT_PADDLE_H_ */
