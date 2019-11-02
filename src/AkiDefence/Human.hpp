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
public:
	Movement generateMovement(GameInfo info, int who);
	void init(GameInfo info);
};
const char gridStr[10] = { ' ','*','S','#','.','@','_' };
void Human::init(GameInfo info)
{

}
Movement Human::generateMovement(GameInfo info,int who)
{
	
	cout << cns::playerNameFull[who] << endl;
	if (info.player[who].hp <= 0)
	{
		cout << "体力用尽无法行动" << endl;
		return Movement::createMovementStay();
	}
	vector<Movement> movement = getEffectiveMovement(info,who);
	cout << "请选择一个操作:" << endl;
	for (int i = 0; i < movement.size(); ++i)
	{
		cout<<i << ".";
		cout << this->printMovement(movement[i]) << endl;
	}
	int t;
	cin >> t;
	return movement[t];
}
