/*
 * Excluder.h
 *
 *  Created on: Jul 14, 2023
 *      Author: nebk2
 */

#ifndef GAMES_EXCLUDER_H_
#define GAMES_EXCLUDER_H_

#include "AARectangle.h"
#include "BoundaryEdge.h"
class Excluder{
public:
	virtual ~Excluder() {}
	void init(const AARectangle& rect, bool reverseNormals = false);
	bool hasCollided(const AARectangle& rect, BoundaryEdge& edge) const;
	Vec2D getCollisionOffset(const AARectangle& rect) const;
	inline const AARectangle& getAARectangle() const {return mAARect;}
	void moveBy(const Vec2D& delta);
	void moveTo(const Vec2D& point);
	const BoundaryEdge& getEdge(EdgeType edge) const;
private:
	void setupEdges();
	AARectangle mAARect;
	BoundaryEdge mEdges[NUM_EDGES];
	bool mReverseNormals;
};

#endif /* GAMES_EXCLUDER_H_ */
