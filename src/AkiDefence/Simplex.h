#pragma once
#include"./MovementGenerator.h"
#include"./Game.h"
#include<iostream>
#include<string>
#include<Windows.h>
#include<map>
using namespace std;
//仅限二人版
struct State
{
	Player player[2];
	int roundLeft;
	bool operator <(const State& rhs)const 
	{
		if (roundLeft < rhs.roundLeft)return true;
		if (roundLeft > rhs.roundLeft)return false;
		if (player[MINORIKO].hp < rhs.player[MINORIKO].hp)return true;
		if (player[MINORIKO].hp > rhs.player[MINORIKO].hp)return false;
		if (player[MARISA].hp < rhs.player[MARISA].hp)return true;
		if (player[MARISA].hp > rhs.player[MARISA].hp)return false;
		if (player[MINORIKO].position.x < rhs.player[MINORIKO].position.x)return true;
		if (player[MINORIKO].position.x > rhs.player[MINORIKO].position.x)return false;
		if (player[MARISA].position.x < rhs.player[MARISA].position.x)return true;
		if (player[MARISA].position.x > rhs.player[MARISA].position.x)return false;
		if (player[MINORIKO].position.y < rhs.player[MINORIKO].position.y)return true;
		if (player[MINORIKO].position.y > rhs.player[MINORIKO].position.y)return false;
		if (player[MARISA].position.y < rhs.player[MARISA].position.y)return true;
		if (player[MARISA].position.y > rhs.player[MARISA].position.y)return false;
		if (player[MINORIKO].have < rhs.player[MINORIKO].have)return true;
		if (player[MINORIKO].have > rhs.player[MINORIKO].have)return false;
		return player[MARISA].have < rhs.player[MARISA].have;
	}
};
class Game;
class SimplexAI:public MovementGenerator
{
private:
	static int id[21 << 1];
	static double a[21][21];
	static map<State, double> avgScore;
	GameInfo startState;
	void printMovements(vector<Movement>& movements); 
	double solve(State state);
	vector<Movement> getEffectiveMovement(const State& state,int side);
	void pivot(int l, int e, int row, int col);
	friend class game;
public:
	void init(GameInfo info);
	Movement generateMovement(GameInfo info);
};
