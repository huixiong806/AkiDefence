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
	int score;
	GridMap<Grid> map;
	int32_t round, roundLimit;
	int collisionDamage;
	int attackDamage;
	int trapDamage;
	int recoverHp;
	std::array<Player, 4> player;
	std::array<int, 4> maxHp;
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
	GameInfo getInfo(int who);
	void newGame(const GameInfo& startState);
};
