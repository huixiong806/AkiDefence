#pragma once
#include"./MovementGenerator.h"
#include"../core/Constant.h"
#include<iostream>
#include<string>
#include<Windows.h>
using namespace std;
class Human:public MovementGenerator
{
private:
	void printInfo(GameInfo& info);
public:
	Movement generateMovement(GameInfo info);
	void init(GameInfo info);
};
const char gridStr[10] = { ' ','*','S','#','.','@','_' };
void Human::init(GameInfo info)
{

}
void Human::printInfo(GameInfo& info)
{
	cout << (info.who?"雾雨魔理沙":"秋穰子")<< endl;
	cout << "第" << info.round << "秒，还剩下" << info.roundLimit - info.round << "秒" << endl;
	cout << "已搬运:" << info.score << endl;
	cout << "HP: 穰子" << info.minoriko.hp<<"/"<<info.minorikoMaxHp << " 魔理沙" << info.marisa.hp<<"/"<<info.marisaMaxHp << endl;
	int64_t colorUp=0;
	if(info.minoriko.hp>0)
		if (info.map[info.minoriko.position.x][info.minoriko.position.y].type == GridType::Trigger)
			colorUp |= info.map[info.minoriko.position.x][info.minoriko.position.y].tag;
	if (info.map[info.marisa.position.x][info.marisa.position.y].type == GridType::Trigger)
		colorUp |= info.map[info.marisa.position.x][info.marisa.position.y].tag;
	for (int i = 0; i < info.map.size().x; ++i)
	{
		for (int j = 0; j < info.map.size().y; ++j)
		{
			if (i == info.marisa.position.x&&j == info.marisa.position.y)
			{
				cout << (info.marisa.have ? 'M' : 'm');
			}
			else if (i == info.minoriko.position.x&&j == info.minoriko.position.y)
			{
				cout << (info.minoriko.have ? 'A' : 'a');
			}
			else
			{
				char out = gridStr[(int)info.map[i][j].type];
				if (info.map[i][j].type == GridType::Trap&&(info.map[i][j].tag|colorUp))
					out = '^';
				if (info.map[i][j].type == GridType::MoveableWall && (info.map[i][j].tag | colorUp))
					out = '#';
				cout << out;
			}
		}
		cout << endl;
	}
}
Movement Human::generateMovement(GameInfo info)
{
	cout << (info.who ? "雾雨魔理沙" : "秋穰子") << endl;
	vector<Movement> movement = info.who ? getMovementForMarisa(info):getMovementForMinoriko(info);
	cout << "请选择一个操作:" << endl;
	for (int i = 0; i < movement.size(); ++i)
	{
		cout<<i << ".";
		cout << this->printMovement(movement[i]) << endl;
	}
	int t;
	cin >> t;
	system("cls");
	return movement[t];
}
