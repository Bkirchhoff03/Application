/*
 * Tetrominos.cpp
 *
 *  Created on: Jul 26, 2023
 *      Author: nebk2
 */
#include "Tetromino.h"
#include "Utils.h"
#include "Tetris.h"
#include "AARectangle.h"
#include "Line2D.h"
#include "GameField.h"
#include "Vec2D.h"
#include <random>
#include <algorithm>
#include <stdint.h>
#include <vector>

Tetromino::Tetromino(const Tetromino &tet) {
	mFillColor = tet.mFillColor;
	mVelocity = tet.mVelocity;
	isLocked = tet.isLocked;
}

const float Tetromino::getBottomPoint() const {
	float biggestYPos = 0;
	for (const Vec2D &point : getRectTopLeftPoints()) {
		if (point.GetY() > biggestYPos) {
			biggestYPos = point.GetY();
		}
	}
	return biggestYPos + squareDim.GetY();
}

const float Tetromino::getTopPoint() const {
	float smallestYPos = 1000;
	for (const Vec2D &point : getRectTopLeftPoints()) {
		if (point.GetY() < smallestYPos) {
			smallestYPos = point.GetY();
		}
	}
	return smallestYPos;
}

const float Tetromino::getLeftPoint() const {
	float smallestXPos = 1000;
	for (const Vec2D &point : getRectTopLeftPoints()) {
		if (point.GetX() < smallestXPos) {
			smallestXPos = point.GetX();
		}
	}
	return smallestXPos;
}

const float Tetromino::getRightPoint() const {
	float biggestXPos = 0;
	for (const Vec2D &point : getRectTopLeftPoints()) {
		if (point.GetX() > biggestXPos) {
			biggestXPos = point.GetX();
		}
	}
	return biggestXPos + squareDim.GetX();
}

void Tetromino::roundPoints() {
	Vec2D square = getSquareSize();
	for (size_t i = 0; i < mPoints.size(); ++i) { //Vec2D &point : mPoints) {
		mPoints[i].SetX(round(mPoints[i].GetX() / square.GetX()) * square.GetX());
		mPoints[i].SetY(round(mPoints[i].GetY() / square.GetY()) * square.GetY());
	}
}
void Tetromino::lock() {
	isLocked = true;
	mVelocity = Vec2D::zero;
}
TetrominoI::TetrominoI() {
	buildTetromino();
}
Vec2D TetrominoI::getCenterPoint() const {
	return mPoints[2];
}
void TetrominoI::draw(Screen &screen) const {
	for (const Vec2D vec : getRectTopLeftPoints()) {
		screen.draw(AARectangle(vec, vec + squareDim), Color::white(), true, mFillColor);
	}
}

const Color& TetrominoI::getFillColor() {
	return mFillColor;
}

void TetrominoI::rotate() {
	Vec2D center = getCenterPoint();
	for (Vec2D &point : mPoints) {
		point.rotate((PI / 2.0f), center);
	}
	rotationState++;
	rotationState %= 4;
	/*if (rotationState == 4) {
	 rotationState = 0;
	 mRightPoint = &mPoints[5];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[5];
	 mTopPoint = &mPoints[0];
	 } else if (rotationState == 3) {
	 mRightPoint = &mPoints[0];
	 mLeftPoint = &mPoints[4];
	 mBottomPoint = &mPoints[0];
	 mTopPoint = &mPoints[9];
	 } else if (rotationState == 2) {
	 mRightPoint = &mPoints[0];
	 mLeftPoint = &mPoints[5];
	 mBottomPoint = &mPoints[4];
	 mTopPoint = &mPoints[5];
	 } else if (rotationState == 1) {
	 mRightPoint = &mPoints[4];
	 mLeftPoint = &mPoints[5];
	 mBottomPoint = &mPoints[5];
	 mTopPoint = &mPoints[0];
	 }*/
	roundPoints();
}

void TetrominoI::moveTo(const Vec2D &point) {

}

/*
 void TetrominoI::moveBy(const Vec2D &delta) {
 for (size_t i = 0; i < mPoints.size(); i++) {
 mPoints[i] += delta;
 std::cout << i << " pos: " << mPoints[i].GetX() << std::endl;
 }
 }
 */

void TetrominoI::teleport() {

}
const std::vector<Vec2D> TetrominoI::getRectTopLeftPoints() const {
	switch (rotationState) {
	case 0: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[2], mPoints[3] };
	}
	case 1: {
		return std::vector<Vec2D> { mPoints[9], mPoints[8], mPoints[7], mPoints[6] };
	}
	case 2: {
		return std::vector<Vec2D> { mPoints[3], mPoints[2], mPoints[1], mPoints[0] };
	}
	case 3: {
		return std::vector<Vec2D> { mPoints[6], mPoints[7], mPoints[8], mPoints[9] };
	}
	default: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[2], mPoints[3] };
	}
	}
}
/*
 Vec2D TetrominoI::findDistanceToBottom(GameField gameField) {
 int i = (int) getBottomPoint().GetY();
 const int x = (int) getBottomPoint().GetX();
 while (i < 21) {
 if (!gameField.getField()[x][i]) {
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }
 i++;
 }
 return Vec2D(0, i - (int) getBottomPoint().GetY());

 }*/
void TetrominoI::buildTetromino() {
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 9, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 9, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY()));
	mFillColor = Color::cyan();
	/*mRightPoint = &mPoints[5];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[5];
	 mTopPoint = &mPoints[0];*/
}

TetrominoJ::TetrominoJ() {
	buildTetromino();
}

Vec2D TetrominoJ::getCenterPoint() const {
	switch (rotationState) {
	case 0: {
		AARectangle rect = { mPoints[2], mPoints[6] };
		return rect.getCenterPoint();
	}
	case 1: {
		AARectangle rect = { mPoints[7], mPoints[3] };
		return rect.getCenterPoint();
	}
	case 2: {
		AARectangle rect = { mPoints[6], mPoints[2] };
		return rect.getCenterPoint();
	}
	case 3: {
		AARectangle rect = { mPoints[3], mPoints[7] };
		return rect.getCenterPoint();
	}
	default: {
		break;
	}
	}
	return Vec2D::zero;
}

void TetrominoJ::draw(Screen &screen) const {
	for (const Vec2D vec : getRectTopLeftPoints()) {
		screen.draw(AARectangle(vec, vec + squareDim), Color::white(), true, mFillColor);
	}
}
const Color& TetrominoJ::getFillColor() {
	return mFillColor;
}
void TetrominoJ::rotate() {
	Vec2D center = getCenterPoint();
	for (Vec2D &point : mPoints) {
		point.rotate((PI / 2.0f), center);
	}
	rotationState++;
	rotationState %= 4;
	/*if (rotationState == 4) {
	 rotationState = 0;
	 mRightPoint = &mPoints[5];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[5];
	 mTopPoint = &mPoints[0];
	 } else if (rotationState == 3) {
	 mRightPoint = &mPoints[8];
	 mLeftPoint = &mPoints[0];
	 mBottomPoint = &mPoints[0];
	 mTopPoint = &mPoints[4];
	 } else if (rotationState == 2) {
	 mRightPoint = &mPoints[0];
	 mLeftPoint = &mPoints[4];
	 mBottomPoint = &mPoints[0];
	 mTopPoint = &mPoints[8];
	 } else if (rotationState == 1) {
	 mRightPoint = &mPoints[0];
	 mLeftPoint = &mPoints[5];
	 mBottomPoint = &mPoints[4];
	 mTopPoint = &mPoints[0];
	 }*/
	roundPoints();
}
void TetrominoJ::moveTo(const Vec2D &point) {
}
/*void TetrominoJ::moveBy(const Vec2D &delta) {
 for (size_t i = 0; i < mPoints.size(); i++) {
 mPoints[i] += delta;
 std::cout << i << " pos: " << mPoints[i].GetX() << std::endl;
 }
 }*/
void TetrominoJ::teleport() {
}
const std::vector<Vec2D> TetrominoJ::getRectTopLeftPoints() const {
	switch (rotationState) {
	case 0: {
		return std::vector<Vec2D> { mPoints[0], mPoints[9], mPoints[2], mPoints[3] };
	}
	case 1: {
		return std::vector<Vec2D> { mPoints[8], mPoints[9], mPoints[7], mPoints[6] };
	}
	case 2: {
		return std::vector<Vec2D> { mPoints[5], mPoints[6], mPoints[7], mPoints[2] };
	}
	case 3: {
		return std::vector<Vec2D> { mPoints[4], mPoints[3], mPoints[2], mPoints[1] };
	}
	default: {
		return std::vector<Vec2D> { mPoints[0], mPoints[9], mPoints[2], mPoints[3] };
	}
	}
}
/*Vec2D TetrominoJ::findDistanceToBottom(GameField gameField) {
 int i = (int) getBottomPoint().GetY();
 const int x = (int) getBottomPoint().GetX();
 while (i < 21) {
 if (!gameField.getField()[x][i]) {
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }
 i++;
 }
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }*/
void TetrominoJ::buildTetromino() {
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY()));
	mFillColor = Color::blue();
	/*mRightPoint = &mPoints[5];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[5];
	 mTopPoint = &mPoints[0];*/
}

TetrominoL::TetrominoL() {
	buildTetromino();
}
Vec2D TetrominoL::getCenterPoint() const {
	switch (rotationState) {
	case 0: {
		AARectangle rect = { mPoints[1], mPoints[7] };
		return rect.getCenterPoint();
	}
	case 1: {
		AARectangle rect = { mPoints[8], mPoints[2] };
		return rect.getCenterPoint();
	}
	case 2: {
		AARectangle rect = { mPoints[7], mPoints[1] };
		return rect.getCenterPoint();
	}
	case 3: {
		AARectangle rect = { mPoints[2], mPoints[8] };
		return rect.getCenterPoint();
	}
	default: {
		break;
	}
	}
	return Vec2D::zero;
}
void TetrominoL::draw(Screen &screen) const {
	for (const Vec2D vec : getRectTopLeftPoints()) {
		screen.draw(AARectangle(vec, vec + squareDim), Color::white(), true, mFillColor);
	}
}
const Color& TetrominoL::getFillColor() {
	return mFillColor;
}
void TetrominoL::rotate() {
	Vec2D center = getCenterPoint();
	for (Vec2D &point : mPoints) {
		point.rotate((PI / 2.0f), center);
	}
	rotationState++;
	rotationState %= 4;
	/*if (rotationState == 4) {
	 rotationState = 0;
	 mRightPoint = &mPoints[6];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[6];
	 mTopPoint = &mPoints[3];
	 } else if (rotationState == 3) {
	 mRightPoint = &mPoints[3];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[6];
	 mTopPoint = &mPoints[0];
	 } else if (rotationState == 2) {
	 mRightPoint = &mPoints[0];
	 mLeftPoint = &mPoints[4];
	 mBottomPoint = &mPoints[4];
	 mTopPoint = &mPoints[6];
	 } else if (rotationState == 1) {
	 mRightPoint = &mPoints[9];
	 mLeftPoint = &mPoints[4];
	 mBottomPoint = &mPoints[0];
	 mTopPoint = &mPoints[6];
	 }*/
	roundPoints();
}
void TetrominoL::moveTo(const Vec2D &point) {
}
/*void TetrominoL::moveBy(const Vec2D &delta) {
 for (size_t i = 0; i < mPoints.size(); i++) {
 mPoints[i] += delta;
 std::cout << i << " pos: " << mPoints[i].GetX() << std::endl;
 }
 }*/
void TetrominoL::teleport() {
}
const std::vector<Vec2D> TetrominoL::getRectTopLeftPoints() const {
	switch (rotationState) {
	case 0: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[2], mPoints[3] };
	}
	case 1: {
		return std::vector<Vec2D> { mPoints[9], mPoints[8], mPoints[7], mPoints[2] };
	}
	case 2: {
		return std::vector<Vec2D> { mPoints[5], mPoints[6], mPoints[7], mPoints[8] };
	}
	case 3: {
		return std::vector<Vec2D> { mPoints[1], mPoints[2], mPoints[4], mPoints[5] };
	}
	default: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[2], mPoints[3] };
	}
	}
}
/*Vec2D TetrominoL::findDistanceToBottom(GameField gameField) {
 int i = (int) getBottomPoint().GetY();
 const int x = (int) getBottomPoint().GetX();
 while (i < 21) {
 if (!gameField.getField()[x][i]) {
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }
 i++;
 }
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }*/
void TetrominoL::buildTetromino() {
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY() * 2));
	mFillColor = Color::orange();
	/*mRightPoint = &mPoints[6];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[6];
	 mTopPoint = &mPoints[3];*/
}

TetrominoO::TetrominoO() {
	buildTetromino();
}
Vec2D TetrominoO::getCenterPoint() const {
	return mPoints[8];
}
void TetrominoO::draw(Screen &screen) const {
	for (const Vec2D vec : getRectTopLeftPoints()) {
		screen.draw(AARectangle(vec, vec + squareDim), Color::white(), true, mFillColor);
	}
}
const Color& TetrominoO::getFillColor() {
	return mFillColor;
}
void TetrominoO::rotate() {
	rotationState++;
	rotationState %= 4;
}
void TetrominoO::moveTo(const Vec2D &point) {
}
/*void TetrominoO::moveBy(const Vec2D &delta) {
 for (size_t i = 0; i < mPoints.size(); i++) {
 mPoints[i] += delta;
 std::cout << i << " pos: " << mPoints[i].GetX() << std::endl;
 }
 }*/
void TetrominoO::teleport() {
}
const std::vector<Vec2D> TetrominoO::getRectTopLeftPoints() const {
	return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[8], mPoints[7] };

}
/*Vec2D TetrominoO::findDistanceToBottom(GameField gameField) {
 int i = (int) getBottomPoint().GetY();
 const int x = (int) getBottomPoint().GetX();
 while (i < 21) {
 if (!gameField.getField()[x][i]) {
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }
 i++;
 }
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }*/
void TetrominoO::buildTetromino() {
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY()));
	mFillColor = Color::yellow();
	/*mRightPoint = &mPoints[4];
	 mLeftPoint = &mPoints[6];
	 mBottomPoint = &mPoints[4];
	 mTopPoint = &mPoints[0];*/
}

TetrominoS::TetrominoS() {
	buildTetromino();
}
Vec2D TetrominoS::getCenterPoint() const {
	switch (rotationState) {
	case 0: {
		AARectangle rect = { mPoints[1], mPoints[7] };
		return rect.getCenterPoint();
	}
	case 1: {
		AARectangle rect = { mPoints[8], mPoints[6] };
		return rect.getCenterPoint();
	}
	case 2: {
		AARectangle rect = { mPoints[7], mPoints[1] };
		return rect.getCenterPoint();
	}
	case 3: {
		AARectangle rect = { mPoints[6], mPoints[8] };
		return rect.getCenterPoint();
	}
	default: {
		break;
	}
	}
	return Vec2D::zero;

}
void TetrominoS::draw(Screen &screen) const {
	for (const Vec2D vec : getRectTopLeftPoints()) {
		screen.draw(AARectangle(vec, vec + squareDim), Color::white(), true, mFillColor);
	}
}
const Color& TetrominoS::getFillColor() {
	return mFillColor;
}

void TetrominoS::rotate() {
	Vec2D center = getCenterPoint();
	for (Vec2D &point : mPoints) {
		point.rotate((PI / 2.0f), center);
	}
	rotationState++;
	rotationState %= 4;
	/*if (rotationState == 4) {
	 rotationState = 0;
	 mRightPoint = &mPoints[5];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[7];
	 mTopPoint = &mPoints[2];
	 } else if (rotationState == 3) {
	 mRightPoint = &mPoints[9];
	 mLeftPoint = &mPoints[4];
	 mBottomPoint = &mPoints[0];
	 mTopPoint = &mPoints[4];
	 } else if (rotationState == 2) {
	 mRightPoint = &mPoints[0];
	 mLeftPoint = &mPoints[4];
	 mBottomPoint = &mPoints[4];
	 mTopPoint = &mPoints[9];
	 } else if (rotationState == 1) {
	 mRightPoint = &mPoints[2];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[4];
	 mTopPoint = &mPoints[0];
	 }*/
	roundPoints();
}
void TetrominoS::moveTo(const Vec2D &point) {
}
/*void TetrominoS::moveBy(const Vec2D &delta) {
 for (size_t i = 0; i < mPoints.size(); i++) {
 mPoints[i] += delta;
 std::cout << i << " pos: " << mPoints[i].GetX() << std::endl;
 }
 }*/
void TetrominoS::teleport() {
}
const std::vector<Vec2D> TetrominoS::getRectTopLeftPoints() const {
	switch (rotationState) {
	case 0: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[2], mPoints[3] };
	}
	case 1: {
		return std::vector<Vec2D> { mPoints[9], mPoints[8], mPoints[6], mPoints[1] };
	}
	case 2: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[2], mPoints[3] };
	}
	case 3: {
		return std::vector<Vec2D> { mPoints[5], mPoints[6], mPoints[7], mPoints[8] };
	}
	default: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[2], mPoints[3] };
	}
	}
}
/*Vec2D TetrominoS::findDistanceToBottom(GameField gameField) {
 int i = (int) getBottomPoint().GetY();
 const int x = (int) getBottomPoint().GetX();
 while (i < 21) {
 if (!gameField.getField()[x][i]) {
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }
 i++;
 }
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }*/
void TetrominoS::buildTetromino() {
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY() * 2));
	mFillColor = Color::green();
	/*mRightPoint = &mPoints[5];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[7];
	 mTopPoint = &mPoints[2];*/
}

TetrominoT::TetrominoT() {
	buildTetromino();
}
Vec2D TetrominoT::getCenterPoint() const {
	switch (rotationState) {
	case 0: {
		AARectangle rect = { mPoints[1], mPoints[7] };
		return rect.getCenterPoint();
	}
	case 1: {
		AARectangle rect = { mPoints[8], mPoints[4] };
		return rect.getCenterPoint();
	}
	case 2: {
		AARectangle rect = { mPoints[7], mPoints[1] };
		return rect.getCenterPoint();
	}
	case 3: {
		AARectangle rect = { mPoints[4], mPoints[8] };
		return rect.getCenterPoint();
	}
	default: {
		break;
	}
	}
	return Vec2D::zero;
}
void TetrominoT::draw(Screen &screen) const {
	for (const Vec2D vec : getRectTopLeftPoints()) {
		screen.draw(AARectangle(vec, vec + squareDim), Color::white(), true, mFillColor);
	}
}
const Color& TetrominoT::getFillColor() {
	return mFillColor;
}
/*
 bool TetrominoT::squaresBelow() {
 }
 */
void TetrominoT::rotate() {
	Vec2D center = getCenterPoint();
	for (Vec2D &point : mPoints) {
		point.rotate((PI / 2.0f), center);
	}
	rotationState++;
	rotationState %= 4;
	/*if (rotationState == 4) {
	 rotationState = 0;
	 mRightPoint = &mPoints[6];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[9];
	 mTopPoint = &mPoints[2];
	 } else if (rotationState == 3) {
	 mRightPoint = &mPoints[9];
	 mLeftPoint = &mPoints[2];
	 mBottomPoint = &mPoints[0];
	 mTopPoint = &mPoints[5];
	 } else if (rotationState == 2) {
	 mRightPoint = &mPoints[0];
	 mLeftPoint = &mPoints[5];
	 mBottomPoint = &mPoints[2];
	 mTopPoint = &mPoints[9];
	 } else if (rotationState == 1) {
	 mRightPoint = &mPoints[2];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[5];
	 mTopPoint = &mPoints[0];
	 }*/
	roundPoints();
}
void TetrominoT::moveTo(const Vec2D &point) {
}
/*void TetrominoT::moveBy(const Vec2D &delta) {
 for (size_t i = 0; i < mPoints.size(); i++) {
 mPoints[i] += delta;
 std::cout << i << " pos: " << mPoints[i].GetX() << std::endl;
 }
 }*/
void TetrominoT::teleport() {
}
const std::vector<Vec2D> TetrominoT::getRectTopLeftPoints() const {
	switch (rotationState) {
	case 0: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[2], mPoints[4] };
	}
	case 1: {
		return std::vector<Vec2D> { mPoints[9], mPoints[8], mPoints[7], mPoints[1] };
	}
	case 2: {
		return std::vector<Vec2D> { mPoints[4], mPoints[6], mPoints[7], mPoints[8] };
	}
	case 3: {
		return std::vector<Vec2D> { mPoints[1], mPoints[3], mPoints[4], mPoints[5] };
	}
	default: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[2], mPoints[4] };
	}
	}

}
/*Vec2D TetrominoT::findDistanceToBottom(GameField gameField) {
 int i = (int) getBottomPoint().GetY();
 const int x = (int) getBottomPoint().GetX();
 while (i < 21) {
 if (!gameField.getField()[x][i]) {
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }
 i++;
 }
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }*/
void TetrominoT::buildTetromino() {
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY() * 2));
	mFillColor = Color::purple();
	/*mRightPoint = &mPoints[6];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[9];
	 mTopPoint = &mPoints[2];*/
}

TetrominoZ::TetrominoZ() {
	buildTetromino();
}
Vec2D TetrominoZ::getCenterPoint() const {
	switch (rotationState) {
	case 0: {
		AARectangle rect = { mPoints[8], mPoints[6] };
		return rect.getCenterPoint();
	}
	case 1: {
		AARectangle rect = { mPoints[7], mPoints[3] };
		return rect.getCenterPoint();
	}
	case 2: {
		AARectangle rect = { mPoints[6], mPoints[8] };
		return rect.getCenterPoint();
	}
	case 3: {
		AARectangle rect = { mPoints[3], mPoints[7] };
		return rect.getCenterPoint();
	}
	default: {
		break;
	}
	}
	return Vec2D::zero;
}
void TetrominoZ::draw(Screen &screen) const {
	for (const Vec2D vec : getRectTopLeftPoints()) {
		screen.draw(AARectangle(vec, vec + squareDim), Color::white(), true, mFillColor);
	}
}
const Color& TetrominoZ::getFillColor() {
	return mFillColor;
}
void TetrominoZ::rotate() {
	Vec2D center = getCenterPoint();
	for (Vec2D &point : mPoints) {
		point.rotate((PI / 2.0f), center);
	}
	rotationState++;
	rotationState %= 4;
	/*if (rotationState == 4) {
	 rotationState = 0;
	 mRightPoint = &mPoints[5];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[7];
	 mTopPoint = &mPoints[0];
	 } else if (rotationState == 3) {
	 mRightPoint = &mPoints[7];
	 mLeftPoint = &mPoints[0];
	 mBottomPoint = &mPoints[0];
	 mTopPoint = &mPoints[5];
	 } else if (rotationState == 2) {
	 mRightPoint = &mPoints[0];
	 mLeftPoint = &mPoints[5];
	 mBottomPoint = &mPoints[0];
	 mTopPoint = &mPoints[7];
	 } else if (rotationState == 1) {
	 mRightPoint = &mPoints[0];
	 mLeftPoint = &mPoints[7];
	 mBottomPoint = &mPoints[5];
	 mTopPoint = &mPoints[0];
	 }*/
	roundPoints();
}
void TetrominoZ::moveTo(const Vec2D &point) {
}
/*void TetrominoZ::moveBy(const Vec2D &delta) {
 for (size_t i = 0; i < mPoints.size(); i++) {
 mPoints[i] += delta;
 std::cout << i << " pos: " << mPoints[i].GetX() << std::endl;
 }
 }*/
void TetrominoZ::teleport() {
}
const std::vector<Vec2D> TetrominoZ::getRectTopLeftPoints() const {
	switch (rotationState) {
	case 0: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[8], mPoints[3] };
	}
	case 1: {
		return std::vector<Vec2D> { mPoints[9], mPoints[8], mPoints[7], mPoints[6] };
	}
	case 2: {
		return std::vector<Vec2D> { mPoints[2], mPoints[5], mPoints[6], mPoints[8] };
	}
	case 3: {
		return std::vector<Vec2D> { mPoints[1], mPoints[2], mPoints[3], mPoints[4] };
	}
	default: {
		return std::vector<Vec2D> { mPoints[0], mPoints[1], mPoints[8], mPoints[3] };
	}
	}
}
/*Vec2D TetrominoZ::findDistanceToBottom(GameField gameField) {
 int i = (int) getBottomPoint().GetY();
 const int x = (int) getBottomPoint().GetX();
 while (i < 21) {
 if (!gameField.getField()[x][i]) {
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }
 i++;
 }
 return Vec2D(0, i - (int) getBottomPoint().GetY());
 }*/
void TetrominoZ::buildTetromino() {
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, 0.0f));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 8, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 7, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY() * 2));
	mPoints.push_back(Vec2D(squareDim.GetX() * 6, squareDim.GetY()));
	mPoints.push_back(Vec2D(squareDim.GetX() * 5, squareDim.GetY()));
	mFillColor = Color::red();
	/*mRightPoint = &mPoints[5];
	 mLeftPoint = &mPoints[9];
	 mBottomPoint = &mPoints[7];
	 mTopPoint = &mPoints[0];*/
}
