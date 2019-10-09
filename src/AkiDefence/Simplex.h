#pragma once
#include"./MovementGenerator.h"
#include"./Game.h"
#include<iostream>
#include<string>
#include<Windows.h>
#include<map>
using namespace std;
struct State
{
	Player minoriko;
	Player marisa;
	int roundLeft;
	bool operator <(const State& rhs)const 
	{
		if (roundLeft < rhs.roundLeft)return true;
		if (roundLeft > rhs.roundLeft)return false;
		if (minoriko.hp < rhs.minoriko.hp)return true;
		if (minoriko.hp > rhs.minoriko.hp)return false;
		if (marisa.hp < rhs.marisa.hp)return true;
		if (marisa.hp > rhs.marisa.hp)return false;
		if (minoriko.position.x < rhs.minoriko.position.x)return true;
		if (minoriko.position.x > rhs.minoriko.position.x)return false;
		if (marisa.position.x < rhs.marisa.position.x)return true;
		if (marisa.position.x > rhs.marisa.position.x)return false;
		if (minoriko.position.y < rhs.minoriko.position.y)return true;
		if (minoriko.position.y > rhs.minoriko.position.y)return false;
		if (marisa.position.y < rhs.marisa.position.y)return true;
		if (marisa.position.y > rhs.marisa.position.y)return false;
		if (minoriko.have < rhs.minoriko.have)return true;
		if (minoriko.have > rhs.minoriko.have)return false;
		return marisa.have < rhs.marisa.have;
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
	void printInfo(GameInfo& info);
	void printMovements(vector<Movement>& movements); 
	double solve(State state);
	vector<Movement> getMovementForMarisa(const State& state);
	vector<Movement> getMovementForMinoriko(const State& state);
	void pivot(int l, int e, int row, int col);
	friend class game;
public:
	void init(GameInfo info);
	Movement generateMovement(GameInfo info);
};
