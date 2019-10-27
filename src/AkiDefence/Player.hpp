#pragma once
#include<vector>
#include"../core/Vector2.h"
enum PlayerName
{
	MINORIKO=0,
	MARISA=1,
	SHIZUHA=2,
	REIMU=3,
};
struct Player
{
	Vec2i position;
	int hp;
	bool have;// «∑Ò”µ”–∫Ï Ì
};