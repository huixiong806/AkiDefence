#include"Constant.h"
namespace cns
{
	bool outofRange(Vec2i pos, Vec2i size)
	{
		return pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y;
	}
	int lowbit(int x)
	{
		return x & -x;
	}
	bool isFriend(int x, int y)
	{
		return x % 2 == y % 2;
	}
}