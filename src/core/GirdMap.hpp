#pragma once
#include <vector>
#include "./Vector2.h"
template<class Type>
class GridMap
{
private:
	std::vector<std::vector<Type>>grid;
	Vec2i gridSize;
public:
	std::vector<Type>& operator [](size_t index)
	{
		return grid[index];
	}
	void resize(Vec2i size)
	{
		gridSize = size;
		grid.resize(size.x);
		for (auto&& item : grid)
			item.resize(size.y);
	}
	Vec2i size()const
	{
		return gridSize;
	}
	void setMap(const std::vector<std::vector<Type>>& map)
	{
		grid = map;
		gridSize = Vec2i(map.size(), map[0].size());
	}
	void setGird(size_t r, size_t c, Type gird_)
	{
		grid[r][c] = gird_;
	}
	Type& getGird(size_t r, size_t c)
	{
		return grid[r][c];
	}
	GridMap& getAllGrids()
	{
		return grid;
	}
};
enum class Gird01 :bool
{
	EMPTY,
	WALL
};
