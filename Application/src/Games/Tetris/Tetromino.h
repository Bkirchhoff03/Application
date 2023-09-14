/*
 * Block.h
 *
 *  Created on: Jul 18, 2023
 *      Author: nebk2
 */

#ifndef GAMES_TETRIS_TETROMINO_H_
#define GAMES_TETRIS_TETROMINO_H_

#include "AARectangle.h"
#include "Color.h"
#include "Line2D.h"
#include "Vec2D.h"
#include "App.h"
#include "GameField.h"
#include <vector>

class Ball;
class Screen;
struct BoundaryEdge;

class Tetromino: public Shape {
public:
	virtual ~Tetromino() {
	}
	virtual void draw(Screen &screen) const = 0;
	Tetromino(const Tetromino& tet);
	Tetromino() {
	}
	inline virtual const Vec2D getPosition() const {
		return mPoints[0];
	}
	virtual const Color& getFillColor() {
		return mFillColor;
	}
	inline void setVelocity(const Vec2D &vel) {
		mVelocity = vel;
	}
	virtual void rotate() {
	}
	inline Vec2D getSquareSize() const {
		return squareDim;
	}
	virtual void moveTo(const Vec2D &point) override = 0;
	//virtual void moveBy(const Vec2D &delta) = 0;
	virtual void teleport() {
	}
	//virtual Vec2D findDistanceToBottom(GameField gameField) = 0;
	virtual inline const bool locked() const {
		return isLocked;
	}
	virtual const std::vector<Vec2D> getRectTopLeftPoints() const = 0;

	inline const Vec2D* getBottomPoint() const {
		return mBottomPoint;
	}
	inline const Vec2D* getTopPoint() const {
		return mTopPoint;
	}
	inline const Vec2D* getLeftPoint() const {
		return mLeftPoint;
	}
	inline const Vec2D* getRightPoint() const {
		return mRightPoint;
	}
	void lock();
protected:
	int rotationState = 0;
	void buildTetromino();
	void roundPoints();

	Vec2D* mRightPoint;
	Vec2D* mLeftPoint;
	Vec2D* mBottomPoint;
	Vec2D* mTopPoint;

	Color mFillColor;
	Vec2D mVelocity;
	const Vec2D squareDim = { App::singleton().width() / 10.0f, App::singleton().height() / 20.0f };
	bool isLocked;
	//std::vector<AARectangle> ghost;
};

class TetrominoI: public Tetromino {
public:
	TetrominoI();
	virtual Vec2D getCenterPoint() const override;
	virtual void draw(Screen &screen) const override;
	virtual const Color& getFillColor() override;
	virtual void rotate() override;
	virtual void moveTo(const Vec2D &point) override;
	//virtual void moveBy(const Vec2D &delta) override;
	virtual void teleport() override;
	//virtual Vec2D findDistanceToBottom(GameField gameField) override;
	void buildTetromino();
	virtual const std::vector<Vec2D> getRectTopLeftPoints() const override;
};
class TetrominoJ: public Tetromino {
public:
	TetrominoJ();
	virtual Vec2D getCenterPoint() const override;
	virtual void draw(Screen &screen) const override;
	virtual const Color& getFillColor() override;
	virtual void rotate() override;
	virtual void moveTo(const Vec2D &point) override;
	//virtual void moveBy(const Vec2D &delta) override;
	virtual void teleport() override;
	//Vec2D findDistanceToBottom(GameField gameField) override;
	void buildTetromino();
	virtual const std::vector<Vec2D> getRectTopLeftPoints() const override;
};
class TetrominoL: public Tetromino {
public:
	TetrominoL();
	virtual Vec2D getCenterPoint() const override;
	virtual void draw(Screen &screen) const override;
	virtual const Color& getFillColor() override;
	virtual void rotate() override;
	virtual void moveTo(const Vec2D &point) override;
	//virtual void moveBy(const Vec2D &delta) override;
	virtual void teleport() override;
	//Vec2D findDistanceToBottom(GameField gameField) override;
	void buildTetromino();
	virtual const std::vector<Vec2D> getRectTopLeftPoints() const override;
};
class TetrominoO: public Tetromino {
public:
	TetrominoO();
	virtual Vec2D getCenterPoint() const override;
	virtual void draw(Screen &screen) const override;
	virtual const Color& getFillColor() override;
	virtual void rotate() override;
	virtual void moveTo(const Vec2D &point) override;
	//virtual void moveBy(const Vec2D &delta) override;
	virtual void teleport() override;
	//Vec2D findDistanceToBottom(GameField gameField) override;
	void buildTetromino();
	virtual const std::vector<Vec2D> getRectTopLeftPoints() const override;
};
class TetrominoS: public Tetromino {
public:
	TetrominoS();
	virtual Vec2D getCenterPoint() const override;
	virtual void draw(Screen &screen) const override;
	virtual const Color& getFillColor() override;
	virtual void rotate() override;
	virtual void moveTo(const Vec2D &point) override;
	//virtual void moveBy(const Vec2D &delta) override;
	virtual void teleport() override;
	//Vec2D findDistanceToBottom(GameField gameField) override;
	void buildTetromino();
	virtual const std::vector<Vec2D> getRectTopLeftPoints() const override;

};
class TetrominoT: public Tetromino {
public:
	TetrominoT();
	virtual Vec2D getCenterPoint() const override;
	virtual void draw(Screen &screen) const override;
	virtual const Color& getFillColor() override;
	virtual void rotate() override;
	virtual void moveTo(const Vec2D &point) override;
	//virtual void moveBy(const Vec2D &delta) override;
	virtual void teleport() override;
	//Vec2D findDistanceToBottom(GameField gameField) override;
	void buildTetromino();
	virtual const std::vector<Vec2D> getRectTopLeftPoints() const override;

};
class TetrominoZ: public Tetromino {
public:
	TetrominoZ();
	virtual Vec2D getCenterPoint() const override;
	virtual void draw(Screen &screen) const override;
	virtual const Color& getFillColor() override;
	virtual void rotate() override;
	virtual void moveTo(const Vec2D &point) override;
	//virtual void moveBy(const Vec2D &delta) override;
	virtual void teleport() override;
	//Vec2D findDistanceToBottom(GameField gameField) override;
	void buildTetromino();
	virtual const std::vector<Vec2D> getRectTopLeftPoints() const override;
};

#endif /* GAMES_TETRIS_TETROMINO_H_ */
