#pragma once
#include"./Map.hpp"
#include"./Player.hpp"
#include"./MovementGenerator.h"
#include "./Simplex.h"
#include<memory>
#include<array>
#include<vector>
class SimplexAI;
class Game
{
private:
	int playerCount=2;//
	bool gameOver;
	int16_t score;
	GridMap<Grid> map;
	int32_t round, roundLimit;
	int16_t collisionDamage;
	int16_t attackDamage;
	int16_t trapDamage;
	int16_t recoverHp;
	std::array<Player, 4> player;
	std::array<int16_t, 4> maxHp;
	std::array<int16_t, 4> bucketVolume;
	std::array<Movement, 4> movement;
	friend class SimplexAI;
public:
	bool timeUp();
	bool gameIsOver();
	void setMovement(const Movement& movement,int side);
	void roundFinish();
	int getScore();
	void printInfo();
	void setPlayer(const Player& player,int side);
	const Player& getPlayerConst(int side);
	GameInfo getInfo();
	void newGame(const GameInfo& startState);
};
