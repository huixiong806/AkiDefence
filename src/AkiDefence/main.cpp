#include<iostream>
#include<memory>
#pragma warning(disable:4996)
#include<fstream>
#include<cstdio>
#include<conio.h>
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
	res.playerCount = 2;
	in >> res.player[MINORIKO].position.x >> res.player[MINORIKO].position.y;
	in >> res.player[MARISA].position.x >> res.player[MARISA].position.y;
	in >> res.player[MINORIKO].hp >> res.player[MARISA].hp;
	in >> res.maxHp[MINORIKO] >> res.maxHp[MARISA];
	in >> res.player[MINORIKO].have >> res.player[MARISA].have;
	in >> res.score;
	in >> res.collisionDamage >> res.attackDamage>>res.trapDamage>>res.recoverHp;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			int type;
			in >> type;
			res.map[i][j].type = (GridType)type;
			if (type == (int)GridType::Shrine || type == (int)GridType::Pile || type >= 4)
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
	cout << "请选择你的阵营，0表示穰子，1表示魔理沙,2表示两个AI对战,实时观看对局,3表示两个AI对战输出到文件" << endl;
	int side;
	cin >> side;
	if(side ==0)minoriko= make_shared<Human>();
	else if(side==1)marisa = make_shared<Human>();
	double sumScore = 0, count = 0;
	int n,roundCount=0;
	cout << "请输入对局数量" << endl;
	cin >> n;
	//获取默认标准显示缓冲区句柄
	HANDLE hOutput;
	COORD coord = { 0,0 };
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	//创建新的缓冲区
	HANDLE hOutBuf = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	//设置新的缓冲区为活动显示缓冲
	if(side==2)
		SetConsoleActiveScreenBuffer(hOutBuf);

	//隐藏两个缓冲区的光标
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = 0;
	cci.dwSize = 1;
	if (side == 2)
	{
		SetConsoleCursorInfo(hOutput, &cci);
		SetConsoleCursorInfo(hOutBuf, &cci);
	}
	//双缓冲处理显示
	DWORD bytes = 0;
	char data[120*30];
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
			if (side == 3)
				freopen("result.txt", "w", stdout);
		}
		double score = 0;
		cout << "第" << roundCount << "局" << endl;
		while (!game.gameIsOver())
		{
			game.printInfo();
			if (side == 0)
			{
				game.setMovement(marisa->generateMovement(game.getInfo(MARISA)),MARISA);
				game.setMovement(minoriko->generateMovement(game.getInfo(MINORIKO)), MINORIKO);
			}
			else
			{
				game.setMovement(minoriko->generateMovement(game.getInfo(MINORIKO)), MINORIKO);
				game.setMovement(marisa->generateMovement(game.getInfo(MARISA)), MARISA);
			}
			if (side == 2)
			{
				ReadConsoleOutputCharacterA(hOutput, data, 120 * 30, coord, &bytes);
				WriteConsoleOutputCharacterA(hOutBuf, data, 120 * 30, coord, &bytes);
				Sleep(3000);
				SetConsoleActiveScreenBuffer(hOutput);
				system("cls");
				SetConsoleActiveScreenBuffer(hOutBuf);
			}
			if(side<3)
				system("cls");
			game.roundFinish();
		}
		cout << "本场比赛结束，魔理沙共搬运了" << game.getScore() << "个红薯" << endl<<endl;
		count++;
		sumScore += game.getScore();
		if (side != 3)
		{
			cout << "还剩下" << n << "次比赛" << endl;
			system("pause");
		}
		
	}
	printf("魔理沙在%d次比赛中平均搬运了%lf个红薯\n", (int)count, sumScore / count);
	cout << "游戏结束" << endl;
	if(side!=3)
		system("pause");
    return 0;
}

