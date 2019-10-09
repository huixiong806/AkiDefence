#include<iostream>
#include<memory>
#pragma warning(disable:4996)
#include<fstream>
#include<time.h>
#include"./Game.h"
#include"./Stayer.hpp"
#include"./Human.hpp"
#include"./Simplex.h"
#include"../core/File.hpp"
using namespace std;
shared_ptr<MovementGenerator> minoriko;
shared_ptr<MovementGenerator> marisa;
GameInfo getStartState(string file)
{
	GameInfo res;
	ifstream in;
	int n, m;
	in.open("./map/"+file, ios::in);
	in >> n >> m;
	res.map.resize(Vec2i(n, m));
	in >> res.round >> res.roundLimit;
	in >> res.minoriko.position.x >> res.minoriko.position.y;
	in >> res.marisa.position.x >> res.marisa.position.y;
	in >> res.minoriko.hp >> res.marisa.hp;
	in >> res.minorikoMaxHp >> res.marisaMaxHp;
	in >> res.minoriko.have >> res.marisa.have;
	in >> res.score;
	in >> res.collisionDamage >> res.attackDamage>>res.trapDamage>>res.recoverHp;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			int type;
			in >> type;
			res.map[i][j].type = (GridType)type;
			if (type == 1 || type == 2 || type >= 4)
				in >> res.map[i][j].tag;
		}
	}
	in.close();
	return res;
}
Game game;
int main()
{	
	srand(time(0));
	minoriko = make_shared<SimplexAI>();
	marisa = make_shared<SimplexAI>();
	cout << "请选择地图，输入地图编号即可" << endl;
	auto maps=FileManager::getAllFiles("./map","m");
	for (int i=0;i<maps.size();++i)
		cout <<i<<"." << maps[i] << endl;
	int mapId;
	cin >> mapId;
	cout << "请选择你的阵营，0表示穰子，1表示魔理沙,2表示两个AI对战(输出对战结果到result.txt)" << endl;
	int side;
	cin >> side;
	if(side ==0)minoriko= make_shared<Human>();
	else if(side==1)marisa = make_shared<Human>();
	double sumScore = 0, count = 0;
	int n,roundCount=0;
	cout << "请输入对局数量" << endl;
	cin >> n;
	while (n--)
	{
		roundCount++;
		game.newGame(getStartState(maps[mapId]));
		if (roundCount == 1)
		{
			cout << "预处理..." << endl;
			minoriko->init(game.getInfo(0));
			marisa->init(game.getInfo(1));
			cout << "预处理结束" << endl;
			if (side == 2)
				freopen("result.txt", "w", stdout);
		}
		double score = 0;
		cout << "第" << roundCount << "局" << endl;
		while (!game.gameIsOver())
		{
			game.printInfo();
			if (side == 0)
			{
				game.setMovementForMarisa(marisa->generateMovement(game.getInfo(1)));
				game.setMovementForMinoriko(minoriko->generateMovement(game.getInfo(0)));
			}
			else
			{
				game.setMovementForMinoriko(minoriko->generateMovement(game.getInfo(0)));
				game.setMovementForMarisa(marisa->generateMovement(game.getInfo(1)));

			}
			game.roundFinish();
		}
		cout << "本场比赛结束，魔理沙共搬运了" << game.getScore() << "个红薯" << endl<<endl;
		count++;
		sumScore += game.getScore();
		if (side != 2)
		{
			cout << "还剩下" << n << "次比赛" << endl;
			system("pause");
		}
		
	}
	printf("魔理沙在%d次比赛中平均搬运了%lf个红薯\n", (int)count, sumScore / count);
	cout << "游戏结束" << endl;
	if(side!=2)
		system("pause");
    return 0;
}

