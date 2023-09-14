/*
 * Ball.h
 *
 *  Created on: Jul 16, 2023
 *      Author: nebk2
 */

#ifndef GAMES_BREAKOUT_BALL_H_
#define GAMES_BREAKOUT_BALL_H_

#include "AARectangle.h"
#include <stdint.h>

class Screen;
struct BoundaryEdge;
class Ball{
public:
	Ball();
	Ball(const Vec2D& pos, float radius);

	void update(uint32_t dt);
	void draw(Screen& screen);
	void makeFlushWithEdge(const BoundaryEdge& edge, Vec2D& pointOnEdge, bool limitToEdge);
	inline void stop(){mVelocity = Vec2D::zero;}
	void moveTo(const Vec2D& point);
	inline const AARectangle getBoundingRect() const {return mBBox;}

	void bounce(const BoundaryEdge& edge);

	inline void setVelocity(const Vec2D& vel) {mVelocity = vel;}
	inline Vec2D getVelocity() {return mVelocity;}
	inline float getRadius() const {return mBBox.getWidth()/2.0f;}
	inline Vec2D getPosition() const {return mBBox.getCenterPoint();}
private:
	AARectangle mBBox;
	Vec2D mVelocity;

	static const float RADIUS;
};



#endif /* GAMES_BREAKOUT_BALL_H_ */
