/*
 * LevelBoundary.h
 *
 *  Created on: Jul 16, 2023
 *      Author: nebk2
 */

#ifndef GAMES_BREAKOUT_LEVELBOUNDARY_H_
#define GAMES_BREAKOUT_LEVELBOUNDARY_H_

#include "Excluder.h"
class AARectangle;
class Ball;

class LevelBoundary{
public:
	LevelBoundary(){}
	LevelBoundary(const AARectangle& boundary);
	bool hasCollided(const Ball& ball, BoundaryEdge& edge);
	inline const AARectangle& getAARectangle() const {return mIncluder.getAARectangle();}
private:
	bool hasCollidedWithEdge(const Ball& ball, const BoundaryEdge& edge) const; //check to see if we collded at all
	Excluder mIncluder;
};



#endif /* GAMES_BREAKOUT_LEVELBOUNDARY_H_ */
