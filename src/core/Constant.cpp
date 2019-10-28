#include"Constant.h"
namespace cns
{
	bool outofRange(Vec2i pos, Vec2i size)
	{
		return pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y;
	}
}