#pragma once
#include<string>
#include "./Vector2.h"
namespace cns
{
	const double eps = 1e-8;
	const Vec2i zero2i = Vec2i(0, 0);
	const Vec2u zero2u = Vec2u(0, 0);
	const Vec2i delta[5] = { Vec2i(-1,0),Vec2i(1,0),Vec2i(0,-1),Vec2i(0,1),Vec2i(0,0)};
	bool outofRange(Vec2i pos, Vec2i size);
	const std::string playerName[4] = { "穰子","魔理沙","静叶","琪露诺" };
	const std::string playerNameFull[4] = { "秋穰子","雾雨魔理沙","秋静叶","琪露诺" };
	int lowbit(int x);
	//表示在游戏中x和y是否为队友
	bool isFriend(int x,int y);
}
