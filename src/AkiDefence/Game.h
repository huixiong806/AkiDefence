#pragma once
#include"./Map.hpp"
#include"./Player.hpp"
#include"./MovementGenerator.h"
#include<memory>
#include<array>
#include<iostream>
#include<vector>
class Game:public GameInfo
{
private:
	bool gameOver;
	std::array<Movement, 4> movement;
public:
	bool timeUp();
	bool gameIsOver();
	void setMovement(const Movement& movement,int side);
	void roundFinish();
	void fastRoundFinishForTwoPlayer()noexcept;
	int getScore();
	void printInfo();
	void setPlayer(const Player& player,int side);
	const Player& getPlayerConst(int side);
	GameInfo getInfo();
	void newGame(const GameInfo& startState);
	void setGameOver(bool gameover);
	//不会复制地图和基本参数的restart
	void restartGame(const GameInfo& startState);
};
