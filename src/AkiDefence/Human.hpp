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
	Movement generateMovement(GameInfo info);
	void init(GameInfo info);
};
const char gridStr[10] = { ' ','*','S','#','.','@','_' };
void Human::init(GameInfo info)
{

}
Movement Human::generateMovement(GameInfo info)
{
	cout << (info.who ? "����ħ��ɳ" : "�����") << endl;
	vector<Movement> movement = getEffectiveMovement(info);
	cout << "��ѡ��һ������:" << endl;
	for (int i = 0; i < movement.size(); ++i)
	{
		cout<<i << ".";
		cout << this->printMovement(movement[i]) << endl;
	}
	int t;
	cin >> t;
	return movement[t];
}
