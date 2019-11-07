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
	//吃红薯补血
	void roundFinish_eatSweetPotato();
	//更新位置(移动和碰撞)
	void roundFinish_updatePosition();
	//大树造成的血量变化以及对琪露诺攻击力的影响，返回值为对琪露诺攻击力的影响
	int roundFinish_treeAffect();
	//投掷红薯
	void roundFinish_throwSweetPotato(int attackDamageChangeForCirno);
	//陷阱的效果
	void roundFinish_trapAffect();
	//红薯的放下和拿起
	void roundFinish_sweetPotatoTransport();
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
