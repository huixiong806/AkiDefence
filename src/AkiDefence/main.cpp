#include<iostream>
#include<memory>
#pragma warning(disable:4996)
#include<fstream>
#include<cstdio>
#include<cassert>
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
template<class T>
void readData(istream& stream,T& target)
{
	stream.read(reinterpret_cast<char*>(&target), sizeof(target));
}
GameInfo getStartState(string file)
{
	GameInfo res;
	ifstream in;
	int16_t map_row, map_col;
	in.open("./map/"+file, ios::in|ios::binary);
	char checkCode[3];
	in.read(checkCode,sizeof(char)*2);
	checkCode[2] = 0;
	if (string(checkCode) != "Ak")
	{
		assert(0);
		return res;
	}
	int16_t bin;
	char ruleId;
	in.read(&ruleId, sizeof(char));
	assert(ruleId == 0);
	readData(in, map_row);
	readData(in, map_col);
	res.map.resize(Vec2i(map_row, map_col));
	readData(in, res.round);
	readData(in, res.roundLimit);
	readData(in, res.score);
	readData(in, res.collisionDamage);
	readData(in, res.attackDamage);
	readData(in, res.trapDamage);
	readData(in, res.recoverHp);
	readData(in, bin);
	char memberAvilible;
	readData(in, memberAvilible);
	assert((memberAvilible & 0x2) != 0);
	res.playerCount = 2;
	for (int i = 0; i < 4; ++i)
	{
		readData(in, res.player[i].position.x);
		readData(in, res.player[i].position.y);
		readData(in, res.player[i].hp);
		readData(in, res.maxHp[i]);
		readData(in, res.player[i].have);
		readData(in, res.bucketVolume[i]);//篮筐容量
	}
	for (int i = 0; i < map_row; ++i)
	{
		for (int j = 0; j < map_col; ++j)
		{
			char type;
			int64_t tag;
			readData(in, type);
			readData(in, tag);
			res.map[i][j].type = (GridType)type;
			if (type == (int)GridType::Shrine || type == (int)GridType::Pile || type >= 4)
				res.map[i][j].tag = tag;
			else res.map[i][j].tag = 0;
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
	marisa = minoriko;
	cout << "请选择地图，输入地图编号即可" << endl;
	auto maps=FileManager::getAllFiles("./map","akm");
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
	HANDLE hOutput[2];
	COORD coord = { 0,0 };
	hOutput[0] = GetStdHandle(STD_OUTPUT_HANDLE);

	//创建新的缓冲区
	hOutput[1] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);


	//隐藏两个缓冲区的光标
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = 0;
	cci.dwSize = 1;
	if (side == 2)
	{
		SetConsoleCursorInfo(hOutput, &cci);
		SetConsoleCursorInfo(hOutput[1], &cci);
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
			minoriko->init(game.getInfo());
			marisa->init(game.getInfo());
			cout << "预处理结束" << endl;
			if (side == 3)
				freopen("result.txt", "w", stdout);
		}
		double score = 0;
		cout << "第" << roundCount << "局" << endl;
		int outputHandle = 0;
		while (!game.gameIsOver())
		{
			game.printInfo();
			if (side == 0)
			{
				game.setMovement(marisa->generateMovement(game.getInfo(), MARISA),MARISA);
				game.setMovement(minoriko->generateMovement(game.getInfo(), MINORIKO), MINORIKO);
			}
			else
			{
				game.setMovement(minoriko->generateMovement(game.getInfo(),MINORIKO), MINORIKO);
				game.setMovement(marisa->generateMovement(game.getInfo(), MARISA), MARISA);
			}
			if (side == 2)
			{
				ReadConsoleOutputCharacterA(hOutput[outputHandle], data, 120 * 30, coord, &bytes);
				WriteConsoleOutputCharacterA(hOutput[outputHandle^1], data, 120 * 30, coord, &bytes);
				//设置新的缓冲区为活动显示缓冲
				SetConsoleActiveScreenBuffer(hOutput[outputHandle^1]);
				
				Sleep(1000);
				
				//SetConsoleActiveScreenBuffer(hOutput);
				//system("cls");
				//SetConsoleActiveScreenBuffer(hOutBuf);
			}
			if (side < 3)
			{
				system("cls");
				outputHandle ^= 1;
				hOutput[outputHandle] = GetStdHandle(STD_OUTPUT_HANDLE);
			}
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

