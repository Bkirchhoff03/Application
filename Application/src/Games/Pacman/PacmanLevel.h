/*
 * PacmanLevel.h
 *
 *  Created on: Aug 14, 2023
 *      Author: nebk2
 */

#ifndef GAMES_PACMAN_PACMANLEVEL_H_
#define GAMES_PACMAN_PACMANLEVEL_H_

#include "Vec2D.h"
#include "Excluder.h"
#include <string>
#include <vector>
#include "PacmanGameUtils.h"
#include "GhostAI.h"
#include "SpriteSheet.h"
#include "BMPImage.h"
#include <random>

class Screen;
class Pacman;
class Ghost;

class PacmanLevel {
public:

	bool init(const std::string &levelPath, const SpriteSheet *noptrSpriteSheet);
	void update(uint32_t dt, Pacman &pacman, std::vector<Ghost> &ghosts, std::vector<GhostAI> &ghostAIs);
	void draw(Screen &screen);

	bool willCollide(const AARectangle &aBBox, PacmanMovement direction) const;
	bool willCollide(const Ghost &ghost, const GhostAI &ghostAI, PacmanMovement direction) const;
	void resetLevel();
	inline Vec2D getLayoutOffset() const {
		return mLayoutOffset;
	}
	inline Vec2D getPacmanSpawnLocation() const {
		return mPacmanSpawnLocation;
	}
	bool isLevelOver() const;
	void increaseLevel();
	void resetToFirstLevel();
	inline const std::vector<Vec2D>& getGhostSpawnPoints() {
		return mGhostsSpawnPoints;
	}
	inline uint32_t getInGameTextYPos() const {
		return mBonusItem.bbox.getTopLeftPoint().GetY();
	}
private:
	struct Tile {
		Vec2D position = Vec2D::zero;
		Vec2D offset = Vec2D::zero;
		int width = 0;
		int collidable = 0;
		int isTeleportTile = 0;
		int excludePelletTile = 0;
		int PacmanSpawnPoint = 0;
		int itemSpawnPoint = 0;
		int blinkySpawnPoint = 0;
		int inkySpawnPoint = 0;
		int pinkySpawnPoint = 0;
		int clydeSpawnPoint = 0;
		int isGate = 0;
		char teleportToSymbol = 0;
		char symbol = '-';
	};

	struct Pellet {
		uint32_t score = 0;
		AARectangle mBBox;
		int powerPellet = 0;
		int eaten = 0;
	};

	struct BonusItem {
		uint32_t score = 0;
		AARectangle bbox;
		int eaten = 0;
		int spawned = 0;
		int spawnTime = -1;
	};

	struct BonusItemLevelProperties {
		uint32_t score = 0;
		std::string spriteName = "";
		uint32_t begin = 0; // which levels does this item spawn at
		uint32_t end = 0;
	};

	bool loadLevel(const std::string &levelPath);
	Tile* getTileForSymbol(char symbol);
	void resetPellets();
	bool hasEatenAllPellets() const;
	size_t numPelletsEaten() const;
	void getBonusItemSpriteName(std::string &spriteName, uint32_t &score) const;
	bool shouldSpawnBonusItem() const;
	void spawnBonusItem();

	BMPImage mBGImage;
	std::vector<Vec2D> mGhostsSpawnPoints;
	std::default_random_engine mGenerator;
	BonusItem mBonusItem;
	std::string mBonusItemSpriteName;
	const SpriteSheet *mnoptrSpriteSheet;
	std::vector<BonusItemLevelProperties> mBonusItemProperties;

	std::vector<Excluder> mWalls;
	std::vector<Tile> mTiles;

	std::vector<Excluder> mGate;

	std::vector<Tile> mExclusionTiles;
	std::vector<Pellet> mPellets;
	Vec2D mPacmanSpawnLocation;

	Vec2D mLayoutOffset;
	size_t mTileHeight;
	int mCurrentLevel;

};




#endif /* GAMES_PACMAN_PACMANLEVEL_H_ */
