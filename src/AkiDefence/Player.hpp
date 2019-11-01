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
	Vec2<int16_t> position;
	int16_t hp;
	int16_t have;//是否拥有红薯
};