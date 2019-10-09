#pragma once
#include"./Map.hpp"
#include"./Player.hpp"
#include"./MovementGenerator.h"
#include "./Simplex.h"
#include<memory>
class SimplexAI;
class Game
{
private:
	Player minoriko;
	Player marisa;
	bool gameOver;
	int score;
	GridMap<Grid> map;
	int32_t round, roundLimit;
	int collisionDamage;
	int attackDamage;
	int trapDamage;
	int recoverHp;
	int marisaMaxHp;
	int minorikoMaxHp;
	Movement movementMarisa;
	Movement movementMinoriko;
	friend class SimplexAI;
public:
	bool timeUp();
	bool gameIsOver();
	void setMovementForMinoriko(const Movement& movement);
	void setMovementForMarisa(const Movement& movement);
	void roundFinish();
	int getScore();
	void printInfo();
	GameInfo getInfo(int who);
	void newGame(const GameInfo& startState);
};
