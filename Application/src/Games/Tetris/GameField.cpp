/*
 * GameField.cpp
 *
 *  Created on: Jul 20, 2023
 *      Author: nebk2
 */

#include "GameField.h"
#include "AARectangle.h"
#include "App.h"
#include "Vec2D.h"
#include "Tetromino.h"
#include "Tetris.h"
#include <utility>
#include <cmath>
#include <iostream>
#include <memory>
using namespace std;

GameField::GameField() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			playField[i][j] = Vec2D((dimensionsBoard.width / 10.0f) * i, (dimensionsBoard.height / 20.0f) * j);
			filledField[i][j] = true;
			std::cout << "i:" << i << " j:" << j << std::endl;
		}
	}
}

void GameField::addTetromino(shared_ptr<Tetromino> tetromino) {
	allTetrominos.push_back(tetromino);
}

const bool GameField::isSquaresBelowFull(shared_ptr<Tetromino> tetromino) {
	Vec2D square = tetromino->getSquareSize();

	for (Vec2D point : tetromino->getRectTopLeftPoints()) {
		bool tF = filledField[(int) round(point.GetX() / square.GetX())][(int) round(
				((point.GetY() / square.GetY()) + 1))];
		if (!tF) {
			tetromino->lock();
			addFills(tetromino);
			std::cout << (int) rintf(point.GetX() / square.GetX()) << " "
					<< (int) rintf(((point.GetY() / square.GetY()) + 1)) << std::endl;
			std::cout << tetromino->getRectTopLeftPoints()[0] << " " << tetromino->getRectTopLeftPoints()[1] << " "
					<< tetromino->getRectTopLeftPoints()[2] << " " << tetromino->getRectTopLeftPoints()[3] << " "
					<< std::endl;

			for (auto &row : filledField) {
				for (auto column : row) {
					std::cout << column << " ";
				}
				std::cout << std::endl;
			}

			return true;
		}
	}
	return false;
}

void GameField::addFills(shared_ptr<Tetromino> tetromino) {
	Vec2D square = tetromino->getSquareSize();
	if (tetromino->locked()) {
		const std::vector<Vec2D> vec = tetromino->getRectTopLeftPoints();
		for (const Vec2D point : vec) {
			filledField[(int) (round((point.GetX()) / square.GetX()))][(int) (round((point.GetY()) / square.GetY()))] =
					false;
		}
		tetromino = nullptr;
	}
}

void GameField::clear() {
	allTetrominos.clear();
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			filledField[i][j] = true;
		}
	}
}
void GameField::drawField(Screen &screen) {
	for (shared_ptr<Tetromino> tet : allTetrominos) {
		tet->draw(screen);
	}
}
