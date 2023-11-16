/*
 * TeamAgainst.h
 *
 *  Created on: Sep 22, 2023
 *      Author: nebk2
 */

#ifndef GAMES_SOCCER_TEAMAGAINST_H_
#define GAMES_SOCCER_TEAMAGAINST_H_


#include "Vec2D.h"
#include "Excluder.h"
#include <string>
#include <vector>
#include "SoccerGameUtils.h"
#include "DefenderAI.h"
#include "SpriteSheet.h"
#include "BMPImage.h"
#include "Utils.h"
#include <random>

class Screen;
class Player;
class Defender;

class TeamAgainst {
public:
	bool init(const std::string &levelPath, const SpriteSheet *noptrSpriteSheet);
	void update(uint32_t dt, Player &player, std::vector<Defender> &defenders, std::vector<DefenderAI> &defenderAIs);
	void draw(Screen &screen);

	bool willCollide(const AARectangle &aBBox, PlayerMovement direction) const;
	bool willCollide(const Defender &defender, const DefenderAI &defenderAI, PlayerMovement direction) const;
	void resetLevel();
	inline Vec2D getLayoutOffset() const {
		return mLayoutOffset;
	}
	inline Vec2D getPlayerSpawnLocation() const {
		return mPlayerSpawnLocation;
	}
	void makeDefenderZones(std::vector<Defender> &defenders, std::vector<DefenderAI> &defenderAIs);
	bool isGameOver() const;
	void increaseLevel();
	void resetToFirstGame();
	inline Size getBounds() const {
		return bounds;
	}
	inline const std::vector<Vec2D>& getDefenderSpawnPoints() {
		return mDefendersSpawnPoints;
	}
	inline uint32_t getInGameTextYPos() const {
		return mPlayerSpawnLocation.GetY();
	}
private:
	struct Tile {
		Vec2D position = Vec2D::zero;
		Vec2D offset = Vec2D::zero;
		//int width = 0;
		int collidablePlayer = 0;
		int collidableBall = 0;
		int collidableGoalie = 0;
		int isOutOfBoundsTile = 0;
		int playerSpawnPoint = 0;
		int goalkeeperSpawnPoint = 0;
		int centerBackSpawnPoint = 0;
		int rightBackSpawnPoint = 0;
		int leftBackSpawnPoint = 0;
		int centerDefensiveMidfielderSpawnPoint = 0;
		int ballSpawnPoint = 0;
		int isGoal = 0;
		char teleportToSymbol = 0;
		char symbol = '-';
	};

	bool loadLevel(const std::string &levelPath);
	Tile* getTileForSymbol(char symbol);

	BMPImage mBGImage;
	std::vector<Vec2D> mDefendersSpawnPoints;
	std::default_random_engine mGenerator;
	const SpriteSheet *mnoptrSpriteSheet;
	std::vector<Excluder> mPlayerBoundaries;
	std::vector<Excluder> mBallBoundaries;
	std::vector<Excluder> mGoalieBoundaries;
	std::vector<Excluder> mGoal;

	std::vector<Tile> mTiles;

	std::vector<Tile> mExclusionTiles;
	Vec2D mPlayerSpawnLocation;

	Vec2D mLayoutOffset;
	size_t mTileHeight;
	size_t mTileWidth;
	float meterWidth = 52.5;
	float meterLength = 68.58;
	float pixelWidth = 162;
	float pixelHeight = 224;
	Size bounds;
	float metersPerPixel = 0.315117394;
};

#endif /* GAMES_SOCCER_TEAMAGAINST_H_ */
