/*
 * SoccerBall.h
 *
 *  Created on: Nov 8, 2023
 *      Author: nebk2
 */

#ifndef GAMES_SOCCER_SOCCERBALL_H_
#define GAMES_SOCCER_SOCCERBALL_H_
#include "Ball.h"

class Screen;
struct BoundaryEdge;
class SoccerBall: Ball {
	SoccerBall();
	SoccerBall(const Vec2D &pos, float radius);

	void update(uint32_t dt);
	void draw(Screen &screen);

	void bounce(const BoundaryEdge &edge);

	
private:
	AARectangle mBBox;
	Vec2D mVelocity;

	static const float RADIUS;
	static const float FRICTION;
};



#endif /* GAMES_SOCCER_SOCCERBALL_H_ */
