/*
 * GameField.h
 *
 *  Created on: Jul 19, 2023
 *      Author: nebk2
 */

#ifndef GAMES_TETRIS_GAMEFIELD_H_
#define GAMES_TETRIS_GAMEFIELD_H_

#include "Vec2D.h"
#include "AARectangle.h"
#include "Screen.h"
#include "Utils.h"
#include <vector>
#include <cmath>
#include <unordered_map>
#include <memory>
using namespace std;
class Tetromino;

class GameField{
public:
	GameField();
	inline const auto getField() {return playField;}
	inline const std::vector<shared_ptr<Tetromino>> getAllTetrominos() const {
		return allTetrominos;
	}
	void addTetromino(shared_ptr<Tetromino> tetromino);
	const bool isSquaresBelowFull(shared_ptr<Tetromino> tetromino);
	void addFills(std::shared_ptr<Tetromino> tetromino);
	void clear();
	void drawField(Screen &screen);
private:
	void makePlayField();
	std::vector<shared_ptr<Tetromino>> allTetrominos;
	Vec2D playField[10][20];
	bool filledField[10][20];
	Size dimensionsBoard = { 144, 288 };
};



#endif /* GAMES_TETRIS_GAMEFIELD_H_ */
