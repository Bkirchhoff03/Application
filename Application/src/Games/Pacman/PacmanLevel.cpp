/*
 * PacmanLevel.cpp
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#include "PacmanLevel.h"
#include "FileCommandLoader.h"
#include "Screen.h"
#include "Pacman.h"
#include "App.h"
#include "Circle.h"
#include "Ghost.h"
#include <cassert>

namespace {
const uint32_t NUM_LEVELS = 256;
const uint32_t SPRITE_HEIGHT = 16;
const uint32_t SPRITE_WIDTH = 16;
}

bool PacmanLevel::init(const std::string &levelPath, const SpriteSheet *noptrSpriteSheet) {
	mCurrentLevel = 0;
	mnoptrSpriteSheet = noptrSpriteSheet;
	mBonusItemSpriteName = "";
	std::random_device r;
	mGenerator.seed(r());
	mGhostsSpawnPoints.resize(NUM_GHOSTS);
	bool levelLoaded = loadLevel(levelPath);
	if (levelLoaded) {
		resetLevel();
	}
	return levelLoaded;
}
void PacmanLevel::update(uint32_t dt, Pacman &pacman, std::vector<Ghost> &ghosts, std::vector<GhostAI> &ghostAIs) {
	for (const Excluder &wall : mWalls) {
		BoundaryEdge edge;
		if (wall.hasCollided(pacman.getBoundingBox(), edge)) {
			Vec2D offset = wall.getCollisionOffset(pacman.getBoundingBox());
			pacman.moveBy(offset);
			pacman.stop();
		}

		for (Ghost &ghost : ghosts) {
			if (wall.hasCollided(ghost.getBoundingBox(), edge)) {
				Vec2D offset = wall.getCollisionOffset(ghost.getBoundingBox());
				ghost.moveBy(offset);
				ghost.stop();
			}
		}
	}

	for (const Excluder &gate : mGate) {
		BoundaryEdge edge;
		if (gate.hasCollided(pacman.getBoundingBox(), edge)) {
			Vec2D offset = gate.getCollisionOffset(pacman.getBoundingBox());
			pacman.moveBy(offset);
			pacman.stop();
		}

		for (size_t i = 0; i < NUM_GHOSTS; ++i) {
			GhostAI &ghostAI = ghostAIs[i];
			Ghost &ghost = ghosts[i];

			if (!(ghostAI.wantsToLeavePen() || ghostAI.isEnteringPen())
					&& gate.hasCollided(ghost.getBoundingBox(), edge)) {
				Vec2D offset = gate.getCollisionOffset(ghost.getBoundingBox());
				ghost.moveBy(offset);
				ghost.stop();
			}
		}
	}

	for (Tile t : mTiles) {
		if (t.isTeleportTile) {
			AARectangle teleportTileAABB(t.position, t.width, static_cast<float>(mTileHeight));

			Tile *teleportToTile = getTileForSymbol(t.teleportToSymbol);
			assert(teleportToTile);
			if (teleportToTile->isTeleportTile) {
				if (teleportTileAABB.intersects(pacman.getBoundingBox())) {
					pacman.moveTo(teleportToTile->position + teleportToTile->offset);
				}
				for (Ghost &ghost : ghosts) {
					if (teleportTileAABB.intersects(ghost.getBoundingBox())) {
						ghost.moveTo(teleportToTile->position + teleportToTile->offset);
					}
				}
			}
		}
	}

	for (Pellet &pellet : mPellets) {
		if (!pellet.eaten) {
			if (pacman.getEatingBoundingBox().intersects(pellet.mBBox)) {
				pellet.eaten = true;

				pacman.ateItem(pellet.score);

				if (pellet.powerPellet) {
					pacman.resetGhostEatenMultiplier();
					for (Ghost &ghost : ghosts) {
						ghost.setStateToVulnerable();
					}
				}
			}
		}
	}

	if (shouldSpawnBonusItem()) {
		spawnBonusItem();
	}

	if (mBonusItem.spawned && !mBonusItem.eaten) {
		if (pacman.getEatingBoundingBox().intersects(mBonusItem.bbox)) {
			mBonusItem.eaten = true;
			pacman.ateItem(mBonusItem.score);
		}
	}
}

void PacmanLevel::draw(Screen &screen) {
	Sprite bgSprite;
	bgSprite.width = mBGImage.getWidth();
	bgSprite.height = mBGImage.getHeight();

	screen.draw(mBGImage, bgSprite, Vec2D::zero);
	for (const Pellet &pellet : mPellets) {
		if (!pellet.eaten) {
			if (!pellet.powerPellet) {
				screen.draw(pellet.mBBox, Color::white());
			} else {
				Circle c(pellet.mBBox.getCenterPoint(), pellet.mBBox.getWidth() / 2);
				screen.draw(c, Color::white(), true, Color::white());
			}
		}
	}

	if (mBonusItem.spawned && !mBonusItem.eaten) {
		screen.draw(*mnoptrSpriteSheet, mBonusItemSpriteName, mBonusItem.bbox.getTopLeftPoint());
	}
}

bool PacmanLevel::willCollide(const AARectangle &aBBox, PacmanMovement direction) const {
	AARectangle bbox = aBBox;

	bbox.moveBy(getMovementVector(direction));

	BoundaryEdge edge;

	for (const Excluder &wall : mWalls) {
		if (wall.hasCollided(bbox, edge)) {
			return true;
		}
	}
	for (const Excluder &gate : mGate) {
		if (gate.hasCollided(bbox, edge)) {
			return true;
		}
	}
	return false;
}

bool PacmanLevel::willCollide(const Ghost &ghost, const GhostAI &ghostAI, PacmanMovement direction) const {
	AARectangle bbox = ghost.getBoundingBox();

	bbox.moveBy(getMovementVector(direction));
	BoundaryEdge edge;
	for (const Excluder &wall : mWalls) {
		if (wall.hasCollided(bbox, edge)) {
			return true;
		}
	}
	for (const Excluder &gate : mGate) {
		if (!(ghostAI.isEnteringPen() || ghostAI.wantsToLeavePen()) && gate.hasCollided(bbox, edge)) {
			return true;
		}
	}
	return false;
}

bool PacmanLevel::shouldSpawnBonusItem() const {
	auto numEaten = numPelletsEaten();
	return !mBonusItem.spawned && numEaten >= mBonusItem.spawnTime;
}

void PacmanLevel::spawnBonusItem() {
	mBonusItem.spawned = 1;
	mBonusItem.eaten = 0;
}

void PacmanLevel::getBonusItemSpriteName(std::string &spriteName, uint32_t &score) const {
	for (const BonusItemLevelProperties &properties : mBonusItemProperties) {
		if (mCurrentLevel >= properties.begin && mCurrentLevel <= properties.end) {
			spriteName = properties.spriteName;
			score = properties.score;
			return;
		}
	}
}

void PacmanLevel::resetLevel() {
	resetPellets();

	std::uniform_int_distribution<size_t> distribution(20, mPellets.size() - 50);
	mBonusItem.spawnTime = static_cast<int>(distribution(mGenerator));

	getBonusItemSpriteName(mBonusItemSpriteName, mBonusItem.score);

}
bool PacmanLevel::hasEatenAllPellets() const {
	return numPelletsEaten() >= mPellets.size() - 4; // super pellets
}
size_t PacmanLevel::numPelletsEaten() const {
	size_t numEaten = 0;
	for (const Pellet &pellet : mPellets) {
		if (!pellet.powerPellet && pellet.eaten) {
			++numEaten;
		}
	}
	return numEaten;
}
bool PacmanLevel::isLevelOver() const {
	return hasEatenAllPellets();
}
void PacmanLevel::increaseLevel() {
	mCurrentLevel++;
	if (mCurrentLevel > NUM_LEVELS) {
		mCurrentLevel = 1;
	}
	resetLevel();
}
void PacmanLevel::resetToFirstLevel() {
	mCurrentLevel = 1;
	resetLevel();
}
void PacmanLevel::resetPellets() {
	mPellets.clear();
	const uint32_t PELLET_SIZE = 2;
	const uint32_t PADDING = static_cast<uint32_t>(mTileHeight);

	uint32_t startingY = mLayoutOffset.GetY() + PADDING + mTileHeight - 1;
	uint32_t startingX = PADDING + 3;

	const uint32_t LEVEL_HEIGHT = mLayoutOffset.GetY() + 32 * mTileHeight;

	Pellet p;
	p.score = 10;

	uint32_t row = 0;
	for (uint32_t y = startingY; y < LEVEL_HEIGHT; y += PADDING, ++row) {
		for (uint32_t x = startingX, col = 0; x < App::singleton().width(); x += PADDING, ++col) {
			if (row == 0 || row == 22) {
				if (col == 0 || col == 25) {
					p.powerPellet = 1;
					p.score = 50;
					p.mBBox = AARectangle(Vec2D(x - 3, y - 3), mTileHeight, mTileHeight);
					mPellets.push_back(p);

					p.powerPellet = 0;
					p.score = 10;
					continue;
				}
			}
			AARectangle rect = AARectangle(Vec2D(x, y), PELLET_SIZE, PELLET_SIZE);
			bool found = false;

			for (const Excluder &wall : mWalls) {
				if (wall.getAARectangle().intersects(rect)) {
					found = true;
					break;
				}
			}
			if (!found) {
				for (const Tile &excludedPelletTile : mExclusionTiles) {
					if (excludedPelletTile.excludePelletTile) {
						AARectangle tileAABB(excludedPelletTile.position, excludedPelletTile.width, mTileHeight);
						if (tileAABB.intersects(rect)) {
							found = true;
							break;
						}
					}
				}
			}

			if (!found) {
				p.mBBox = rect;
				mPellets.push_back(p);
			}
		}
	}
}

bool PacmanLevel::loadLevel(const std::string &levelPath) {
	FileCommandLoader fileLoader;

	std::string bgImageName;
	Command bgImageCommand;
	bgImageCommand.command = "bg_image";
	bgImageCommand.parseFunc = [this, &bgImageName](ParseFuncParams params) {
		bgImageName = FileCommandLoader::readString(params);
		bool loaded = mBGImage.load(App::singleton().getBasePath() + std::string("Assets/") + bgImageName);
		assert(loaded && "Didn't load the bg image");
	};
	fileLoader.addCommand(bgImageCommand);

	Command tileWidthCommand;
	tileWidthCommand.command = "tile_width";
	tileWidthCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().width = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileWidthCommand);

	Command tileHeightCommand;
	tileHeightCommand.command = "tile_height";
	tileHeightCommand.parseFunc = [this](ParseFuncParams params) {
		mTileHeight = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileHeightCommand);

	Command tileCommand;
	tileCommand.command = "tile";
	tileCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.push_back(Tile());
	};
	fileLoader.addCommand(tileCommand);

	Command tileSymbolCommand;
	tileSymbolCommand.command = "tile_symbol";
	tileSymbolCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().symbol = FileCommandLoader::readChar(params);
	};
	fileLoader.addCommand(tileSymbolCommand);

	Command tileCollisionCommand;
	tileCollisionCommand.command = "tile_collision";
	tileCollisionCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().collidable = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileCollisionCommand);

	Vec2D layoutOffset;

	Command layoutOffsetCommand;
	layoutOffsetCommand.command = "layout_offset";
	layoutOffsetCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params) {
		mLayoutOffset = FileCommandLoader::readSize(params);
		layoutOffset = mLayoutOffset;
	};

	fileLoader.addCommand(layoutOffsetCommand);

	Command tileIsTeleportTileCommand;
	tileIsTeleportTileCommand.command = "tile_is_teleport_tile";
	tileIsTeleportTileCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().isTeleportTile = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileIsTeleportTileCommand);

	Command tileToTeleportToCommand;
	tileToTeleportToCommand.command = "tile_teleport_to_symbol";
	tileToTeleportToCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().teleportToSymbol = FileCommandLoader::readChar(params);
	};
	fileLoader.addCommand(tileToTeleportToCommand);

	Command tileOffsetCommand;
	tileOffsetCommand.command = "tile_offset";
	tileOffsetCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().offset = FileCommandLoader::readSize(params);
	};
	fileLoader.addCommand(tileOffsetCommand);

	Command tileExcludePelletCommand;
	tileExcludePelletCommand.command = "tile_exclude_pellet";
	tileExcludePelletCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().excludePelletTile = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileExcludePelletCommand);

	Command tilePacmanSpawnPointCommand;
	tilePacmanSpawnPointCommand.command = "tile_pacman_spawn_point";
	tilePacmanSpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().PacmanSpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tilePacmanSpawnPointCommand);

	Command tileItemSpawnPointCommand;
	tileItemSpawnPointCommand.command = "tile_item_spawn_point";
	tileItemSpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().itemSpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileItemSpawnPointCommand);

	Command tileBlinkySpawnPointCommand;
	tileBlinkySpawnPointCommand.command = "tile_blinky_spawn_point";
	tileBlinkySpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().blinkySpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileBlinkySpawnPointCommand);

	Command tileInkySpawnPointCommand;
	tileInkySpawnPointCommand.command = "tile_inky_spawn_point";
	tileInkySpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().inkySpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileInkySpawnPointCommand);

	Command tilePinkySpawnPointCommand;
	tilePinkySpawnPointCommand.command = "tile_pinky_spawn_point";
	tilePinkySpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().pinkySpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tilePinkySpawnPointCommand);

	Command tileClydeSpawnPointCommand;
	tileClydeSpawnPointCommand.command = "tile_clyde_spawn_point";
	tileClydeSpawnPointCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().clydeSpawnPoint = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileClydeSpawnPointCommand);

	Command tileGateCommand;
	tileGateCommand.command = "tile_is_gate";
	tileGateCommand.parseFunc = [this](ParseFuncParams params) {
		mTiles.back().isGate = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(tileGateCommand);



	Command layoutCommand;
	layoutCommand.command = "layout";
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params) {
		int startingX = layoutOffset.GetX();
		for (size_t c = 0; c < params.line.length(); ++c) {
			Tile *tile = getTileForSymbol(params.line[c]);
			if (tile) {
				tile->position = Vec2D(startingX, layoutOffset.GetY());
				if (tile->isGate > 0) {
					Excluder gate;

					gate.init(
							AARectangle(Vec2D(startingX, layoutOffset.GetY()), tile->width,
									static_cast<int>(mTileHeight)));
					mGate.push_back(gate);
				} else if (tile->collidable > 0) {
					Excluder wall;
					wall.init(
							AARectangle(Vec2D(startingX, layoutOffset.GetY()), tile->width,
									static_cast<int>(mTileHeight)));
					mWalls.push_back(wall);
				}

				if (tile->PacmanSpawnPoint > 0) {
					mPacmanSpawnLocation = Vec2D(startingX + tile->offset.GetX(),
							layoutOffset.GetY() + tile->offset.GetY());
				} else if (tile->itemSpawnPoint > 0) {
					mBonusItem.bbox = AARectangle(
							Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY()),
							SPRITE_WIDTH, SPRITE_HEIGHT);
				} else if (tile->blinkySpawnPoint > 0) {
					mGhostsSpawnPoints[BLINKY] = Vec2D(startingX + tile->offset.GetX() + 1,
							layoutOffset.GetY() + tile->offset.GetY());
				} else if (tile->pinkySpawnPoint > 0) {
					mGhostsSpawnPoints[PINKY] = Vec2D(startingX + tile->offset.GetX(),
							layoutOffset.GetY() + tile->offset.GetY());
				} else if (tile->inkySpawnPoint > 0) {
					mGhostsSpawnPoints[INKY] = Vec2D(startingX + tile->offset.GetX() + 1,
							layoutOffset.GetY() + tile->offset.GetY());
				} else if (tile->clydeSpawnPoint > 0) {
					mGhostsSpawnPoints[CLYDE] = Vec2D(startingX + tile->offset.GetX(),
							layoutOffset.GetY() + tile->offset.GetY());
				}

				if (tile->excludePelletTile > 0) {
					mExclusionTiles.push_back(*tile);
				}

				startingX += tile->width;
			}
		}
		layoutOffset += Vec2D(0, mTileHeight);
	};

	fileLoader.addCommand(layoutCommand);

	Command bonusItemCommand;
	bonusItemCommand.command = "bonus_item";
	bonusItemCommand.parseFunc = [this](ParseFuncParams params) {
		BonusItemLevelProperties newProperty;
		mBonusItemProperties.push_back(newProperty);
	};
	fileLoader.addCommand(bonusItemCommand);

	Command bonusItemSpriteNameCommand;
	bonusItemSpriteNameCommand.command = "bonus_item_sprite_name";
	bonusItemSpriteNameCommand.parseFunc = [this](ParseFuncParams params) {
		mBonusItemProperties.back().spriteName = FileCommandLoader::readString(params);
	};
	fileLoader.addCommand(bonusItemSpriteNameCommand);

	Command bonusItemScoreCommand;
	bonusItemScoreCommand.command = "bonus_item_score";
	bonusItemScoreCommand.parseFunc = [this](ParseFuncParams params) {
		mBonusItemProperties.back().score = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(bonusItemScoreCommand);

	Command bonusItemBeginLevelCommand;
	bonusItemBeginLevelCommand.command = "bonus_item_begin_level";
	bonusItemBeginLevelCommand.parseFunc = [this](ParseFuncParams params) {
		mBonusItemProperties.back().begin = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(bonusItemBeginLevelCommand);

	Command bonusItemEndLevelCommand;
	bonusItemEndLevelCommand.command = "bonus_item_end_level";
	bonusItemEndLevelCommand.parseFunc = [this](ParseFuncParams params) {
		mBonusItemProperties.back().end = FileCommandLoader::readInt(params);
	};
	fileLoader.addCommand(bonusItemEndLevelCommand);

	return fileLoader.loadFile(levelPath);
}
PacmanLevel::Tile* PacmanLevel::getTileForSymbol(char symbol) {
	for (size_t i = 0; i < mTiles.size(); ++i) {
		if (mTiles[i].symbol == symbol) {
			return &mTiles[i];
		}
	}
	return nullptr;
}

