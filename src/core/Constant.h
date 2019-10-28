#pragma once
#include "./Vector2.h"
namespace cns
{
	const double eps = 1e-8;
	const Vec2i zero2i = Vec2i(0, 0);
	const Vec2u zero2u = Vec2u(0, 0);
	const Vec2i delta[5] = { Vec2i(-1,0),Vec2i(1,0),Vec2i(0,-1),Vec2i(0,1),Vec2i(0,0)};
	bool outofRange(Vec2i pos, Vec2i size);
}
