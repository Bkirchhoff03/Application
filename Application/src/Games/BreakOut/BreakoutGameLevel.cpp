/*
 * BreakoutGameLevel.cpp
 *
 *  Created on: Jul 17, 2023
 *      Author: nebk2
 */

#include "BreakoutGameLevel.h"
#include "Ball.h"
#include "App.h"
#include "FileCommandLoader.h"

BreakoutGameLevel::BreakoutGameLevel() {

}
void BreakoutGameLevel::init(const AARectangle &boundary) {
	createDefaultLevel(boundary);
}
void BreakoutGameLevel::load(const std::vector<Block> &blocks) {
	mBlocks.clear();
	mBlocks = blocks;
}

void BreakoutGameLevel::update(uint32_t dt, Ball &ball) {
	std::vector<Block> collidedBlocks;
	BoundaryEdge edgeToBounceOffOf;
	Block *noptrBlockToBounceOffOf = nullptr;

	float largestMag = -1.0f;
	for (Block &block : mBlocks) {
		BoundaryEdge edge;

		if (!block.isDestroyed()
				&& block.hasCollided(ball.getBoundingRect(), edge)) {
			collidedBlocks.push_back(block);

			float mag = block.getCollisionOffset(ball.getBoundingRect()).mag();

			if (mag > largestMag) {
				edgeToBounceOffOf = edge;
				noptrBlockToBounceOffOf = &block;
			}
		}

	}

	if (noptrBlockToBounceOffOf != nullptr) {
		noptrBlockToBounceOffOf->bounce(ball, edgeToBounceOffOf);
		noptrBlockToBounceOffOf->reduceHP();
	}

	for (const Block &block : collidedBlocks) {
		BoundaryEdge edge;
		if (block.hasCollided(ball.getBoundingRect(), edge)) {
			Vec2D p;
			ball.makeFlushWithEdge(edge, p, true);
		}
	}
}
void BreakoutGameLevel::draw(Screen &screen) {
	for (Block &block : mBlocks) {
		if (!block.isDestroyed()) {
			block.draw(screen);
		}
	}
}

void BreakoutGameLevel::createDefaultLevel(const AARectangle &boundary) {
	mBlocks.clear();



	const int NUM_BLOCKS_ACROSS = ((int) boundary.getWidth()
			- (2 * (BLOCK_WIDTH)) / BLOCK_WIDTH);
	const int NUM_BLOCK_ROWS = 5;

	float startX = ((int) boundary.getWidth()
			- (NUM_BLOCKS_ACROSS * (BLOCK_WIDTH + 1))) / 2;

	Color colors[NUM_BLOCK_ROWS];
	colors[0] = Color::red();
	colors[1] = Color::magenta();
	colors[2] = Color::yellow();
	colors[3] = Color::green();
	colors[4] = Color::cyan();

	for (int r = 0; r < NUM_BLOCK_ROWS; ++r) {
		AARectangle blockRect = { Vec2D(startX, BLOCK_HEIGHT * (r + 1)),
				BLOCK_WIDTH, BLOCK_HEIGHT };

		for (int c = 0; c < NUM_BLOCKS_ACROSS; ++c) {
			Block b;
			b.init(blockRect, 1, Color::black(), colors[r]);
			mBlocks.push_back(b);
			blockRect.moveBy(Vec2D(BLOCK_WIDTH, 0));
		}
	}

}

bool BreakoutGameLevel::isLevelComplete() const{
	for(size_t i = 0; i < mBlocks.size(); ++i){
		if(!mBlocks[i].isDestroyed() && mBlocks[i].getHP() != Block::UNBREAKABLE){
			return false;
		}
	}
	return true;
}

struct LayoutBlock {
	char symbol = '-';
	int hp = 0;
	Color color = Color::black();
};

LayoutBlock findLayoutBlockForSymbol(const std::vector<LayoutBlock> &blocks,
		char symbol) {
	for (size_t i = 0; i < blocks.size(); ++i) {
		if (blocks[i].symbol == symbol) {
			return blocks[i];
		}
	}
	return LayoutBlock();
}

std::vector<BreakoutGameLevel> BreakoutGameLevel::loadLevelsFromFile(
		const std::string &filePath) {
	std::vector<BreakoutGameLevel> levels;
	std::vector<LayoutBlock> layoutBlocks;
	std::vector<Block> levelBlocks;

	int width = 0;
	int height = 0;

	FileCommandLoader fileLoader;

	Command levelCommand;
	levelCommand.command = "level";
	levelCommand.parseFunc = [&](ParseFuncParams params) {
		if (levels.size() > 0) {
			levels.back().load(levelBlocks);
		}
		layoutBlocks.clear();
		levelBlocks.clear();
		width = 0;
		height = 0;

		BreakoutGameLevel level;
		level.init(
				AARectangle(Vec2D::zero, App::singleton().width(),
						App::singleton().height()));
		levels.push_back(level);
	};

	fileLoader.addCommand(levelCommand);

	Command blockCommand;
	blockCommand.command = "block";
	blockCommand.parseFunc = [&](ParseFuncParams params) {
		LayoutBlock lb;
		layoutBlocks.push_back(lb);
	};

	fileLoader.addCommand(blockCommand);

	Command symbolCommand;
	symbolCommand.command = "symbol";
	symbolCommand.parseFunc = [&](ParseFuncParams params) {
		layoutBlocks.back().symbol = FileCommandLoader::readChar(params);
	};
	fileLoader.addCommand(symbolCommand);

	Command fillColorCommand;
	fillColorCommand.command = "fillcolor";
	fillColorCommand.parseFunc = [&](ParseFuncParams params) {
		layoutBlocks.back().color = FileCommandLoader::readColor(params);
	};
	fileLoader.addCommand(fillColorCommand);

	Command hpCommand;
	hpCommand.command = "hp";
	hpCommand.parseFunc = [&](ParseFuncParams params) {
		layoutBlocks.back().hp = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(hpCommand);

	Command widthCommand;
	widthCommand.command = "width";
	widthCommand.parseFunc = [&](ParseFuncParams params) {
		width = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(widthCommand);

	Command heightCommand;
	heightCommand.command = "height";
	heightCommand.parseFunc = [&](ParseFuncParams params) {
		height = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(heightCommand);

	Command layoutCommand;
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.command = "layout";
	layoutCommand.parseFunc = [&](ParseFuncParams params){

		float startingX = 0;
		AARectangle blockRect(Vec2D(startingX, (params.lineNum+1)*BLOCK_HEIGHT), BLOCK_WIDTH, BLOCK_HEIGHT);
		for(int c = 0; c < params.line.length(); ++c){
			if(params.line[c] != '-'){
				LayoutBlock layoutBlock = findLayoutBlockForSymbol(layoutBlocks, params.line[c]);

				Block b;
				b.init(blockRect, layoutBlock.hp, Color::black(), layoutBlock.color);
				levelBlocks.push_back(b);
			}
			blockRect.moveBy(Vec2D(BLOCK_WIDTH, 0));
		}
	};

	fileLoader.addCommand(layoutCommand);

	fileLoader.loadFile(filePath);

	if(levels.size() > 0){
		levels.back().load(levelBlocks);
	}
	return levels;
}
