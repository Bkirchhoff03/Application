/*
 * BreakoutGameLevel.h
 *
 *  Created on: Jul 17, 2023
 *      Author: nebk2
 */

#ifndef GAMES_BREAKOUT_BREAKOUTGAMELEVEL_H_
#define GAMES_BREAKOUT_BREAKOUTGAMELEVEL_H_

#include "Block.h"
#include <vector>
#include <stdint.h>

class Screen;
class Ball;
class AARectangle;

class BreakoutGameLevel{
public:
	BreakoutGameLevel();
	void init(const AARectangle& boundary);
	void load(const std::vector<Block>& blocks);
	void update(uint32_t dt, Ball& ball);
	void draw(Screen& screen);
	bool isLevelComplete() const;
	static std::vector<BreakoutGameLevel> loadLevelsFromFile(const std::string& filePath);
private:
	static const int BLOCK_WIDTH = 16;
	static const int BLOCK_HEIGHT = 8;
	void createDefaultLevel(const AARectangle& boundary);
	std::vector<Block> mBlocks;
};


#endif /* GAMES_BREAKOUT_BREAKOUTGAMELEVEL_H_ */
